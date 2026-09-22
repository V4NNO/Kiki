#include "dxgicapture.h"

#ifdef Q_OS_WIN

#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>

#include <iterator>

using Microsoft::WRL::ComPtr;

struct DxgiOutputCapture::Impl {
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGIOutputDuplication> duplication;
    ComPtr<ID3D11Texture2D> stagingTexture;
    UINT width = 0;
    UINT height = 0;
};

namespace {
QString hrToString(HRESULT hr)
{
    return QStringLiteral("0x%1").arg(static_cast<quint32>(hr), 8, 16, QLatin1Char('0'));
}
}

DxgiOutputCapture::DxgiOutputCapture()
    : m_impl(new Impl)
{
}

DxgiOutputCapture::~DxgiOutputCapture()
{
    release();
    delete m_impl;
}

bool DxgiOutputCapture::isValid() const
{
    return m_impl->duplication != nullptr;
}

void DxgiOutputCapture::release()
{
    m_impl->stagingTexture.Reset();
    m_impl->duplication.Reset();
    m_impl->context.Reset();
    m_impl->device.Reset();
    m_impl->width = 0;
    m_impl->height = 0;
}

bool DxgiOutputCapture::initialize(void *targetMonitor, const QString &debugName, QString *error)
{
    release();
    const HMONITOR wantedMonitor = static_cast<HMONITOR>(targetMonitor);

    ComPtr<IDXGIFactory1> factory;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        if (error) {
            *error = QStringLiteral("CreateDXGIFactory1 a esuat (%1)").arg(hrToString(hr));
        }
        return false;
    }

    for (UINT adapterIndex = 0;; ++adapterIndex) {
        ComPtr<IDXGIAdapter1> adapter;
        if (factory->EnumAdapters1(adapterIndex, &adapter) == DXGI_ERROR_NOT_FOUND) {
            break;
        }

        for (UINT outputIndex = 0;; ++outputIndex) {
            ComPtr<IDXGIOutput> output;
            if (adapter->EnumOutputs(outputIndex, &output) == DXGI_ERROR_NOT_FOUND) {
                break;
            }
            DXGI_OUTPUT_DESC outputDesc{};
            if (FAILED(output->GetDesc(&outputDesc))) {
                continue;
            }
            if (outputDesc.Monitor != wantedMonitor) {
                continue;
            }
            Q_UNUSED(debugName)

            // Found the matching output. Create a D3D11 device on the
            // adapter that actually owns it (important on multi-GPU
            // machines) and duplicate this specific output.
            static const D3D_FEATURE_LEVEL kFeatureLevels[] = {
                D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
            };
            D3D_FEATURE_LEVEL obtainedLevel;
            hr = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, 0,
                                   kFeatureLevels, static_cast<UINT>(std::size(kFeatureLevels)),
                                   D3D11_SDK_VERSION, &m_impl->device, &obtainedLevel,
                                   &m_impl->context);
            if (FAILED(hr)) {
                if (error) {
                    *error = QStringLiteral("D3D11CreateDevice a esuat (%1)").arg(hrToString(hr));
                }
                release();
                return false;
            }

            ComPtr<IDXGIOutput1> output1;
            hr = output.As(&output1);
            if (FAILED(hr)) {
                if (error) {
                    *error = QStringLiteral("IDXGIOutput1 nu este disponibil (%1)").arg(hrToString(hr));
                }
                release();
                return false;
            }

            hr = output1->DuplicateOutput(m_impl->device.Get(), &m_impl->duplication);
            if (FAILED(hr)) {
                if (error) {
                    *error = QStringLiteral("DuplicateOutput a esuat (%1)").arg(hrToString(hr));
                }
                release();
                return false;
            }

            DXGI_OUTDUPL_DESC duplDesc{};
            m_impl->duplication->GetDesc(&duplDesc);
            m_impl->width = duplDesc.ModeDesc.Width;
            m_impl->height = duplDesc.ModeDesc.Height;

            D3D11_TEXTURE2D_DESC stagingDesc{};
            stagingDesc.Width = m_impl->width;
            stagingDesc.Height = m_impl->height;
            stagingDesc.MipLevels = 1;
            stagingDesc.ArraySize = 1;
            stagingDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            stagingDesc.SampleDesc.Count = 1;
            stagingDesc.Usage = D3D11_USAGE_STAGING;
            stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            hr = m_impl->device->CreateTexture2D(&stagingDesc, nullptr, &m_impl->stagingTexture);
            if (FAILED(hr)) {
                if (error) {
                    *error = QStringLiteral("CreateTexture2D (staging) a esuat (%1)").arg(hrToString(hr));
                }
                release();
                return false;
            }
            return true;
        }
    }

    if (error) {
        *error = QStringLiteral("Niciun output DXGI nu corespunde lui %1").arg(debugName);
    }
    return false;
}

bool DxgiOutputCapture::captureFrame(int timeoutMs, QImage *image, QString *error)
{
    if (!isValid()) {
        if (error) {
            *error = QStringLiteral("Captura DXGI nu este initializata.");
        }
        return false;
    }

    ComPtr<IDXGIResource> desktopResource;
    DXGI_OUTDUPL_FRAME_INFO frameInfo{};
    HRESULT hr = m_impl->duplication->AcquireNextFrame(static_cast<UINT>(timeoutMs), &frameInfo,
                                                       &desktopResource);
    if (hr == DXGI_ERROR_WAIT_TIMEOUT) {
        return false; // No new frame -- desktop was static, not an error.
    }
    if (FAILED(hr)) {
        if (error) {
            *error = QStringLiteral("AcquireNextFrame a esuat (%1)").arg(hrToString(hr));
        }
        // DXGI_ERROR_ACCESS_LOST (mode change, secure desktop, ...) and any
        // other failure both mean this duplication session is dead; the
        // caller is expected to release() and initialize() again.
        return false;
    }

    if (frameInfo.LastPresentTime.QuadPart == 0) {
        // AcquireNextFrame() also wakes up for cursor-only movement (it
        // updates frameInfo.LastMouseUpdateTime even when nothing was
        // actually redrawn) -- LastPresentTime is the field that tells us
        // the desktop image itself changed. Without this check, moving the
        // mouse around with nothing else happening was enough to trigger a
        // full CopyResource+Map+JPEG-encode every tick, which is why CPU
        // usage tracked mouse movement instead of staying near idle like
        // the original.
        m_impl->duplication->ReleaseFrame();
        return false;
    }

    ComPtr<ID3D11Texture2D> acquiredTexture;
    hr = desktopResource.As(&acquiredTexture);
    if (FAILED(hr)) {
        if (error) {
            *error = QStringLiteral("QueryInterface ID3D11Texture2D a esuat (%1)").arg(hrToString(hr));
        }
        m_impl->duplication->ReleaseFrame();
        return false;
    }

    m_impl->context->CopyResource(m_impl->stagingTexture.Get(), acquiredTexture.Get());
    // Release the DXGI frame as soon as the copy is issued -- holding it
    // longer than necessary is exactly the kind of thing that would make
    // this capture method start competing with the compositor again.
    m_impl->duplication->ReleaseFrame();

    D3D11_MAPPED_SUBRESOURCE mapped{};
    hr = m_impl->context->Map(m_impl->stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) {
        if (error) {
            *error = QStringLiteral("Map (staging) a esuat (%1)").arg(hrToString(hr));
        }
        return false;
    }

    // DXGI_FORMAT_B8G8R8A8_UNORM's byte layout (B,G,R,A in memory, little
    // endian) matches QImage::Format_ARGB32 exactly, so this is a plain
    // row-by-row copy, not a channel-swapping conversion.
    QImage frame(static_cast<const uchar *>(mapped.pData), static_cast<int>(m_impl->width),
                static_cast<int>(m_impl->height), static_cast<int>(mapped.RowPitch),
                QImage::Format_ARGB32);
    *image = frame.copy(); // deep copy: mapped.pData is invalid after Unmap

    m_impl->context->Unmap(m_impl->stagingTexture.Get(), 0);
    return true;
}

#else

struct DxgiOutputCapture::Impl {};
DxgiOutputCapture::DxgiOutputCapture() : m_impl(nullptr) {}
DxgiOutputCapture::~DxgiOutputCapture() = default;
bool DxgiOutputCapture::isValid() const { return false; }
void DxgiOutputCapture::release() {}
bool DxgiOutputCapture::initialize(void *, const QString &, QString *error)
{
    if (error) {
        *error = QStringLiteral("DXGI disponibil doar pe Windows.");
    }
    return false;
}
bool DxgiOutputCapture::captureFrame(int, QImage *, QString *error)
{
    if (error) {
        *error = QStringLiteral("DXGI disponibil doar pe Windows.");
    }
    return false;
}

#endif
