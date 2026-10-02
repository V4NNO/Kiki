#include "mainwindow.h"

#include "devicedetailview.h"
#include "devicetilewidget.h"
#include "historyview.h"
#include "monitorwidget.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QCursor>
#include <QDateTime>
#include <QEvent>
#include <QDialog>
#include <QFile>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QLocale>
#include <QMessageBox>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QRect>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSet>
#include <QSettings>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QStyledItemDelegate>
#include <QStatusBar>
#include <QStyle>
#include <QSysInfo>
#include <QtMath>
#include <QTabBar>
#include <QTimer>
#include <QTreeWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include <algorithm>
#include <climits>
#include <cmath>

namespace {
// The real Kickidler viewer's own limit, confirmed from its extracted QML
// (ConstJs.qml): `var maxEmployeeCellsPerTrackerTab = 25;` -- a flat count,
// not columns*rows. Past this, the "+" tile just shows a "limit reached"
// tooltip and does nothing (see AddDeviceTileWidget::setLimitReached) --
// it does not auto-open a new tab.
constexpr int kMaxTilesPerTab = 25;

// A marker file rather than a QSettings key: "Delete local cache" needs to
// survive the very QSettings().clear() it's scheduling, and a QSettings key
// would get wiped along with everything else. Checked once at startup (see
// MainWindow's constructor) before loadSettings() runs.
QString pendingCacheClearMarkerPath()
{
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return dir + QStringLiteral("/pending_cache_clear");
}
}

namespace {
class FullScreenFrame final : public QWidget
{
public:
    explicit FullScreenFrame(const QImage &image)
        : m_image(image)
    {
        setAttribute(Qt::WA_DeleteOnClose);
        setWindowTitle(QStringLiteral("Vizualizare pe tot ecranul — Esc sau dublu clic pentru inchidere"));
        setCursor(Qt::PointingHandCursor);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(5, 8, 16));
        QSize targetSize = m_image.size();
        targetSize.scale(size(), Qt::KeepAspectRatio);
        const QRect target(QPoint((width() - targetSize.width()) / 2,
                                  (height() - targetSize.height()) / 2), targetSize);
        painter.drawImage(target, m_image);
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            close();
        }
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Escape) {
            close();
        } else {
            QWidget::keyPressEvent(event);
        }
    }

private:
    QImage m_image;
};
}

namespace {
// employeePicker/department/employee/Header.qml: a 7px ring bullet (green
// #28A36F online, red #ED5B4C offline), the name in white, and a right-
// aligned "online from / offline since <date>" at 30% opacity. Roles:
// Qt::UserRole = offline (bool), Qt::UserRole+1 = the timestamp text.
class EmployeeRowDelegate final : public QStyledItemDelegate
{
public:
    static constexpr int kOfflineRole = Qt::UserRole;
    static constexpr int kTimestampRole = Qt::UserRole + 1;
    static constexpr int kDeviceKeyRole = Qt::UserRole + 2;

    using QStyledItemDelegate::QStyledItemDelegate;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QSize s = QStyledItemDelegate::sizeHint(option, index);
        s.setHeight(qMax(s.height(), 26));
        return s;
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        const QRect r = option.rect;
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(r, QColor(255, 255, 255, 26));
        } else if (option.state & QStyle::State_MouseOver) {
            painter->fillRect(r, QColor(255, 255, 255, 20));
        }
        // Ring bullet.
        const bool offline = index.data(kOfflineRole).toBool();
        const QColor ring = offline ? QColor(0xED, 0x5B, 0x4C) : QColor(0x28, 0xA3, 0x6F);
        const int d = 7;
        const int bx = r.left() + 10;
        const int by = r.center().y() - d / 2;
        QPen pen(ring);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(QRectF(bx + 0.5, by + 0.5, d, d));

        const int textLeft = bx + d + 6;
        const int rightPad = 10;
        QFont nameFont(QStringLiteral("Roboto"));
        nameFont.setPixelSize(12);
        nameFont.setBold(true);
        const QString timestamp = index.data(kTimestampRole).toString();
        QFont tsFont(QStringLiteral("Roboto"));
        tsFont.setPixelSize(11);
        // Timestamp right-aligned, white @30%.
        int tsWidth = 0;
        if (!timestamp.isEmpty()) {
            tsWidth = QFontMetrics(tsFont).horizontalAdvance(timestamp) + 12;
            painter->setFont(tsFont);
            QColor ts(Qt::white);
            ts.setAlphaF(0.3);
            painter->setPen(ts);
            painter->drawText(QRect(r.right() - rightPad - tsWidth, r.top(), tsWidth, r.height()),
                              Qt::AlignRight | Qt::AlignVCenter, timestamp);
        }
        // Name, elided to the space left of the timestamp.
        painter->setFont(nameFont);
        painter->setPen(Qt::white);
        const int nameWidth = qMax(10, r.right() - rightPad - tsWidth - 4 - textLeft);
        painter->drawText(QRect(textLeft, r.top(), nameWidth, r.height()),
                          Qt::AlignLeft | Qt::AlignVCenter,
                          QFontMetrics(nameFont).elidedText(index.data(Qt::DisplayRole).toString(),
                                                            Qt::ElideMiddle, nameWidth));
        painter->restore();
    }
};

// TrackerPanel.qml's Grids/Filters: icon (b_<kind>_normal/hovered/pressed)
// + Fonts.rr_medium_b text, #a2a2a4 normally, white hovered, #717276 pressed.
class TrackerPanelButton final : public QPushButton
{
public:
    TrackerPanelButton(const QString &kind, const QString &text, QWidget *parent)
        : QPushButton(text, parent), m_kind(kind)
    {
        setObjectName(QStringLiteral("trackerPanelButton"));
        setIconSize(QSize(19, 17));
        setCursor(Qt::PointingHandCursor);
        updateIcon();
    }

protected:
    void enterEvent(QEnterEvent *event) override
    {
        m_hovered = true;
        updateIcon();
        QPushButton::enterEvent(event);
    }
    void leaveEvent(QEvent *event) override
    {
        m_hovered = false;
        updateIcon();
        QPushButton::leaveEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        QPushButton::mousePressEvent(event);
        updateIcon();
    }
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QPushButton::mouseReleaseEvent(event);
        updateIcon();
    }

private:
    void updateIcon()
    {
        const QString state = isDown() ? QStringLiteral("pressed")
                            : (m_hovered || isChecked()) ? QStringLiteral("hovered")
                                                         : QStringLiteral("normal");
        setIcon(QIcon(QStringLiteral(":/tracker/panel/b_%1_%2.png").arg(m_kind, state)));
    }

    QString m_kind;
    bool m_hovered = false;
};
}

// TopPanel.qml's primary activity row (Tracker/Reports/History/Remote
// Control): icon (topPanel/b_<kind>_<state>.png, 20x17) + Fonts bold 11pt
// text, 5px apart, no background at all -- only the icon/text color change
// on hover/press/active. Text color: activated #fcfcfc, pressed #b7b7b7,
// hovered #cfcfcf, normal #287853 (dark green, so an inactive tab all but
// disappears into the header until touched). File scope (not the anonymous
// namespace above) so mainwindow.h can forward-declare it and hold typed
// m_trackerNavButton/m_historyNavButton pointers -- setActivated() isn't
// part of QPushButton's own interface.
class TopNavButton final : public QPushButton
{
public:
    TopNavButton(const QString &kind, const QString &text, QWidget *parent)
        : QPushButton(text, parent), m_kind(kind)
    {
        setObjectName(QStringLiteral("topNavButton"));
        setIconSize(QSize(20, 17));
        setCursor(Qt::PointingHandCursor);
        setCheckable(true);
        setFocusPolicy(Qt::NoFocus);
        setStyleSheet(QStringLiteral(
            "QPushButton#topNavButton { border: none; background: transparent; padding: 10px 0; "
            "font-size: 11pt; font-weight: 700; }"));
        updateState();
    }

    // The real "currentActivity" concept -- exactly one of these is
    // activated at a time; MainWindow drives it explicitly instead of
    // relying on checked/exclusive-group semantics, since Reports/Remote
    // Control never actually become active (no backing feature).
    void setActivated(bool activated)
    {
        setChecked(activated);
        updateState();
    }

protected:
    void enterEvent(QEnterEvent *event) override
    {
        m_hovered = true;
        updateState();
        QPushButton::enterEvent(event);
    }
    void leaveEvent(QEvent *event) override
    {
        m_hovered = false;
        updateState();
        QPushButton::leaveEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        QPushButton::mousePressEvent(event);
        updateState();
    }
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QPushButton::mouseReleaseEvent(event);
        updateState();
    }

private:
    void updateState()
    {
        QString state;
        QColor color;
        if (!isEnabled()) {
            state = QStringLiteral("normal");
            color = QColor(0x1c, 0x4d, 0x36); // dimmer still than normal -- disabled
        } else if (isDown()) {
            state = QStringLiteral("pressed");
            color = QColor(0xb7, 0xb7, 0xb7);
        } else if (isChecked()) {
            state = QStringLiteral("activated");
            color = QColor(0xfc, 0xfc, 0xfc);
        } else if (m_hovered) {
            state = QStringLiteral("hovered");
            color = QColor(0xcf, 0xcf, 0xcf);
        } else {
            state = QStringLiteral("normal");
            color = QColor(0x28, 0x78, 0x53);
        }
        setIcon(QIcon(QStringLiteral(":/topPanel/b_%1_%2.png").arg(m_kind, state)));
        setStyleSheet(QStringLiteral(
            "QPushButton#topNavButton { border: none; background: transparent; padding: 10px 0; "
            "font-size: 11pt; font-weight: 700; color: %1; }").arg(color.name()));
    }

    QString m_kind;
    bool m_hovered = false;
};

// One thumbnail in the Grids panel -- utils/GridsPanel.qml's ImageButton
// delegate: gridsPanel/<key>_<state>.png, checkable (exclusive within the
// panel), no text.
class GridLayoutButton final : public QPushButton
{
public:
    GridLayoutButton(const QString &assetKey, const QSize &thumbnailSize, QWidget *parent)
        : QPushButton(parent), m_assetKey(assetKey)
    {
        setObjectName(QStringLiteral("gridLayoutButton"));
        setIconSize(thumbnailSize);
        setFixedSize(thumbnailSize + QSize(2, 20)); // topMargin/bottomMargin: 10 each
        setCheckable(true);
        setCursor(Qt::PointingHandCursor);
        setFlat(true);
        setStyleSheet(QStringLiteral("QPushButton#gridLayoutButton { border: none; background: transparent; }"));
        updateIcon();
    }

protected:
    void enterEvent(QEnterEvent *event) override
    {
        m_hovered = true;
        updateIcon();
        QPushButton::enterEvent(event);
    }
    void leaveEvent(QEvent *event) override
    {
        m_hovered = false;
        updateIcon();
        QPushButton::leaveEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        QPushButton::mousePressEvent(event);
        updateIcon();
    }
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QPushButton::mouseReleaseEvent(event);
        updateIcon();
    }
    void checkStateSet() override
    {
        QPushButton::checkStateSet();
        updateIcon();
    }

private:
    void updateIcon()
    {
        const QString state = isDown() ? QStringLiteral("pressed")
                            : isChecked() ? QStringLiteral("activated")
                            : m_hovered ? QStringLiteral("hovered")
                                        : QStringLiteral("normal");
        setIcon(QIcon(QStringLiteral(":/gridsPanel/%1_%2.png").arg(m_assetKey, state)));
    }

    QString m_assetKey;
    bool m_hovered = false;
};

// utils/GridsPanel.qml: a 200px-wide popup (gridsPanel/bg.png background,
// #45464e-ish rounded panel) with the Horizontal/Vertical switch on top
// (hmon/vmon icons + label, per positionMonitorsSwitch) and a scrollable
// column of layout thumbnails below (h_* or v_* depending on the switch).
class GridsPanel final : public QWidget
{
    Q_OBJECT

public:
    GridsPanel(const QString &currentLayout, bool currentVertical, QWidget *parent)
        : QWidget(parent, Qt::Popup)
        , m_currentLayout(currentLayout)
        , m_vertical(currentVertical)
    {
        setAttribute(Qt::WA_TranslucentBackground);
        setFixedWidth(200);

        auto *root = new QVBoxLayout(this);
        root->setContentsMargins(13, 10, 3, 10);
        root->setSpacing(10);

        auto *switchRow = new QWidget(this);
        auto *switchLayout = new QVBoxLayout(switchRow);
        switchLayout->setContentsMargins(10, 0, 0, 0);
        switchLayout->setSpacing(7);
        m_horizontalRow = new QPushButton(switchRow);
        m_horizontalRow->setFlat(true);
        m_horizontalRow->setCursor(Qt::PointingHandCursor);
        m_horizontalRow->setStyleSheet(QStringLiteral("text-align: left; border: none; background: transparent;"));
        connect(m_horizontalRow, &QPushButton::clicked, this, [this] { setVertical(false); });
        m_verticalRow = new QPushButton(switchRow);
        m_verticalRow->setFlat(true);
        m_verticalRow->setCursor(Qt::PointingHandCursor);
        m_verticalRow->setStyleSheet(QStringLiteral("text-align: left; border: none; background: transparent;"));
        connect(m_verticalRow, &QPushButton::clicked, this, [this] { setVertical(true); });
        switchLayout->addWidget(m_horizontalRow);
        switchLayout->addWidget(m_verticalRow);
        root->addWidget(switchRow);

        m_scroll = new QScrollArea(this);
        m_scroll->setWidgetResizable(true);
        m_scroll->setFrameShape(QFrame::NoFrame);
        m_scroll->setStyleSheet(QStringLiteral("background: transparent;"));
        m_scroll->viewport()->setStyleSheet(QStringLiteral("background: transparent;"));
        m_thumbHolder = new QWidget(m_scroll);
        m_thumbLayout = new QVBoxLayout(m_thumbHolder);
        m_thumbLayout->setContentsMargins(0, 0, 0, 0);
        m_thumbLayout->setSpacing(0);
        m_thumbLayout->setAlignment(Qt::AlignHCenter);
        m_scroll->setWidget(m_thumbHolder);
        root->addWidget(m_scroll, 1);

        rebuild();
        setMaximumHeight(560);
    }

signals:
    void layoutChosen(const QString &layout, bool vertical);

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0x46, 0x47, 0x4e));
        painter.drawRoundedRect(rect(), 4, 4);
    }

private:
    void setVertical(bool vertical)
    {
        if (m_vertical == vertical) {
            return;
        }
        m_vertical = vertical;
        rebuild();
    }

    void rebuild()
    {
        updateSwitchRow();

        QLayoutItem *item = nullptr;
        while ((item = m_thumbLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // application/Model/tracker/GridsPanel.qml's h_layouts/v_layouts.
        static const QStringList kHorizontal = {
            QStringLiteral("simple"), QStringLiteral("f44"), QStringLiteral("f33"),
            QStringLiteral("p10f22"), QStringLiteral("f22"), QStringLiteral("f22p02f22"),
            QStringLiteral("f22p20f22"), QStringLiteral("f34")};
        static const QStringList kVertical = {
            QStringLiteral("simple"), QStringLiteral("f22"), QStringLiteral("p01f22"),
            QStringLiteral("f22p03f22"), QStringLiteral("p02f22"), QStringLiteral("p03f22"),
            QStringLiteral("f22p02f22")};
        const QStringList &names = m_vertical ? kVertical : kHorizontal;
        const QSize thumbnailSize = m_vertical ? QSize(70, 99) : QSize(99, 75);
        for (const QString &name : names) {
            const QString assetKey = (m_vertical ? QStringLiteral("v_") : QStringLiteral("h_")) + name;
            auto *button = new GridLayoutButton(assetKey, thumbnailSize, m_thumbHolder);
            button->setChecked(name == m_currentLayout);
            connect(button, &QPushButton::clicked, this, [this, name] {
                m_currentLayout = name;
                emit layoutChosen(name, m_vertical);
                for (int i = 0; i < m_thumbLayout->count(); ++i) {
                    if (auto *other = qobject_cast<QPushButton *>(m_thumbLayout->itemAt(i)->widget())) {
                        other->setChecked(other == sender());
                    }
                }
            });
            m_thumbLayout->addWidget(button);
        }
    }

    void updateSwitchRow()
    {
        const QIcon hIcon(QStringLiteral(":/gridsPanel/hmon_%1.png").arg(!m_vertical ? "active" : "inactive"));
        const QIcon vIcon(QStringLiteral(":/gridsPanel/vmon_%1.png").arg(m_vertical ? "active" : "inactive"));
        m_horizontalRow->setIcon(hIcon);
        m_horizontalRow->setIconSize(QSize(11, 9));
        m_horizontalRow->setText(QStringLiteral("  Horizontal"));
        m_horizontalRow->setStyleSheet(QStringLiteral(
            "text-align: left; border: none; background: transparent; color: %1;")
            .arg(!m_vertical ? QStringLiteral("white") : QStringLiteral("#8d8d90")));
        m_verticalRow->setIcon(vIcon);
        m_verticalRow->setIconSize(QSize(7, 11));
        m_verticalRow->setText(QStringLiteral("  Vertical"));
        m_verticalRow->setStyleSheet(QStringLiteral(
            "text-align: left; border: none; background: transparent; color: %1;")
            .arg(m_vertical ? QStringLiteral("white") : QStringLiteral("#8d8d90")));
    }

    QString m_currentLayout;
    bool m_vertical;
    QPushButton *m_horizontalRow = nullptr;
    QPushButton *m_verticalRow = nullptr;
    QScrollArea *m_scroll = nullptr;
    QWidget *m_thumbHolder = nullptr;
    QVBoxLayout *m_thumbLayout = nullptr;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // "Delete local cache on Viewer restart" scheduled a clear on a
    // previous run (see the menu action) -- honor it now, before anything
    // reads settings this session.
    const QString marker = pendingCacheClearMarkerPath();
    if (QFile::exists(marker)) {
        QSettings().clear();
        QFile::remove(marker);
    }

    m_tabs.append(TrackerTab{QStringLiteral("New tab_1"), {}});

    buildInterface();
    applyStyle();
    loadSettings();
    applyStyle(); // re-apply now that loadSettings() may have set m_fontScale
    if (m_simpleModeAction) {
        const QSignalBlocker blocker(m_simpleModeAction);
        m_simpleModeAction->setChecked(m_simpleMode);
    }
    qApp->installEventFilter(this);

    connect(&m_connection, &ViewerConnection::statusChanged,
            this, &MainWindow::updateStatus);
    connect(&m_connection, &ViewerConnection::agentIdentified,
            this, &MainWindow::setAgentIdentity);
    connect(&m_connection, &ViewerConnection::monitorDiscovered,
            this, &MainWindow::addMonitor);
    connect(&m_connection, &ViewerConnection::frameReady,
            this, &MainWindow::updateFrame);
    connect(&m_connection, &ViewerConnection::metadataChanged,
            this, &MainWindow::updateMetadata);
    connect(&m_connection, &ViewerConnection::protocolError,
            this, &MainWindow::showProtocolError);
    // Categories for the tiles' rating colors (global + each employee's own).
    connect(&m_connection, &ViewerConnection::historyCategoriesReceived, this,
            [this](quint32, const QHash<QString, QString> &categories) {
                m_globalCategories = categories;
                for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
                    refreshTileActivity(it.key());
                }
            });
    connect(&m_connection, &ViewerConnection::historyEmployeeCategoriesReceived, this,
            [this](quint32 streamId, const QHash<QString, QString> &categories) {
                const quint32 deviceKey = m_streamDeviceKey.value(streamId, 0);
                if (deviceKey != 0) {
                    m_deviceEmployeeCategories.insert(deviceKey, categories);
                    refreshTileActivity(deviceKey);
                }
            });
    auto *categoryRefresh = new QTimer(this);
    categoryRefresh->setInterval(30000);
    connect(categoryRefresh, &QTimer::timeout, this, [this] {
        for (quint32 streamId : std::as_const(m_devicePrimaryStream)) {
            m_connection.requestCategories(streamId);
        }
    });
    categoryRefresh->start();

    // utils/NoCNodeConnectionBlocker.qml: modal after 60s with no server
    // connection -- started on connect attempt, canceled on success/
    // disconnect (see connectOrDisconnect()/updateStatus()).
    m_noConnectionTimer = new QTimer(this);
    m_noConnectionTimer->setSingleShot(true);
    m_noConnectionTimer->setInterval(60000);
    connect(m_noConnectionTimer, &QTimer::timeout, this, [this]() {
        if (m_connection.isConnected()) {
            return;
        }
        QMessageBox::warning(
            this, QStringLiteral("Fara conexiune"),
            QStringLiteral("Nu s-a putut stabili conexiunea cu serverul de peste 60 de secunde. "
                           "Verifica host/port/token din Connection... si incearca din nou."));
    });

    // utils/NoEmployeesAssignedInformer.qml: informer after 5s if still
    // connected but no device has shown up yet.
    m_noEmployeesTimer = new QTimer(this);
    m_noEmployeesTimer->setSingleShot(true);
    m_noEmployeesTimer->setInterval(5000);
    connect(m_noEmployeesTimer, &QTimer::timeout, this, [this]() {
        if (m_connection.isConnected() && m_devicePrimaryStream.isEmpty()) {
            m_agentLabel->show();
        }
    });

    // Auto-connect on launch using the saved host/token, instead of making
    // the user open Connection... and click "Connect now" every single
    // time -- explicitly requested for this personal, single-user setup.
    if (!m_hostEdit->text().isEmpty() && !m_tokenEdit->text().isEmpty()) {
        QTimer::singleShot(0, this, &MainWindow::connectOrDisconnect);
    }
}

void MainWindow::connectOrDisconnect()
{
    if (m_connection.isConnected()) {
        m_connection.stopDemo();
        m_connection.disconnectFromAgent();
        clearMonitors();
        updateStatus(QStringLiteral("Deconectat"), false);
        m_noConnectionTimer->stop();
        m_noEmployeesTimer->stop();
        m_agentLabel->hide();
        return;
    }
    clearMonitors();
    saveSettings();
    m_connection.connectToAgent(m_hostEdit->text(),
                                static_cast<quint16>(m_portSpin->value()),
                                m_tokenEdit->text(), m_tlsCheck->isChecked(),
                                m_fingerprintEdit->text());
    m_noConnectionTimer->start();
}

void MainWindow::autoConnect(const QString &host, quint16 port, const QString &token, bool useTls)
{
    m_hostEdit->setText(host);
    m_portSpin->setValue(port);
    m_tokenEdit->setText(token);
    m_tlsCheck->setChecked(useTls);
    connectOrDisconnect();
}

void MainWindow::startDemo()
{
    clearMonitors();
    m_connection.startDemo();
}

void MainWindow::showSettings()
{
    m_settingsDialog->show();
    m_settingsDialog->raise();
    m_settingsDialog->activateWindow();
}

void MainWindow::showPreferences()
{
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle(QStringLiteral("Settings"));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMinimumWidth(360);
    auto *layout = new QVBoxLayout(dialog);
    auto *form = new QFormLayout;

    auto *languageCombo = new QComboBox(dialog);
    languageCombo->addItem(QStringLiteral("Romana"), QStringLiteral("ro"));
    languageCombo->addItem(QStringLiteral("English"), QStringLiteral("en"));
    languageCombo->setCurrentIndex(languageCombo->findData(m_language));
    form->addRow(QStringLiteral("Language:"), languageCombo);

    auto *fontScaleSpin = new QSpinBox(dialog);
    fontScaleSpin->setRange(75, 150);
    fontScaleSpin->setSuffix(QStringLiteral("%"));
    fontScaleSpin->setSingleStep(5);
    fontScaleSpin->setValue(qRound(m_fontScale * 100.0));
    form->addRow(QStringLiteral("Font scale:"), fontScaleSpin);

    auto *tooltipsCheck = new QCheckBox(QStringLiteral("Afiseaza tooltip-uri"), dialog);
    tooltipsCheck->setChecked(m_tooltipsEnabled);
    form->addRow(QString(), tooltipsCheck);

    layout->addLayout(form);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::close);
    connect(buttons, &QDialogButtonBox::accepted, this,
            [this, dialog, languageCombo, fontScaleSpin, tooltipsCheck]() {
                m_language = languageCombo->currentData().toString();
                QLocale::setDefault(QLocale(m_language == QStringLiteral("en") ? QLocale::English
                                                                                : QLocale::Romanian));
                m_fontScale = fontScaleSpin->value() / 100.0;
                m_tooltipsEnabled = tooltipsCheck->isChecked();
                applyStyle();
                saveSettings();
                dialog->close();
            });
    layout->addWidget(buttons);
    dialog->show();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (!m_tooltipsEnabled && event->type() == QEvent::ToolTip) {
        return true; // swallow -- matches Settings.qml's Tooltips toggle
    }
    if (event->type() == QEvent::Resize) {
        if (watched == m_gridScrollPage) {
            if (auto *shadow = m_gridScrollPage->findChild<QWidget *>(QStringLiteral("gridShadow"))) {
                shadow->setGeometry(0, 0, m_gridScrollPage->width(), shadow->height());
            }
        } else if (watched == m_gridScroll) {
            // QuadratorGrid.qml re-runs mkCells4View on every size change.
            relayoutCurrentTab();
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::showAbout()
{
    // topPanel/AboutDialog.qml: a dedicated 450x358 dialog (Version/OS/
    // Support/links/copyright), not a generic QMessageBox::about() text box.
    auto *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(QStringLiteral("About the program"));
    dialog->setFixedSize(450, 358);
    auto *layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(10);

    auto *brand = new QLabel(QStringLiteral("Kiki Viewer"), dialog);
    brand->setStyleSheet(QStringLiteral("font-size: 16pt; font-weight: 700; color: #1da06f;"));
    layout->addWidget(brand);

    auto addRow = [layout, dialog](const QString &label, const QString &value) {
        auto *row = new QHBoxLayout;
        auto *labelWidget = new QLabel(label, dialog);
        labelWidget->setStyleSheet(QStringLiteral("color: #9fa5ae;"));
        labelWidget->setFixedWidth(90);
        row->addWidget(labelWidget);
        auto *valueWidget = new QLabel(value, dialog);
        valueWidget->setWordWrap(true);
        valueWidget->setTextInteractionFlags(Qt::TextSelectableByMouse);
        row->addWidget(valueWidget, 1);
        layout->addLayout(row);
    };
    addRow(QStringLiteral("Version:"), QCoreApplication::applicationVersion());
    addRow(QStringLiteral("OS:"),
          QStringLiteral("%1 (%2)").arg(QSysInfo::prettyProductName(), QSysInfo::currentCpuArchitecture()));
    addRow(QStringLiteral("Qt:"), QStringLiteral(QT_VERSION_STR));
    addRow(QStringLiteral("Support:"), QStringLiteral("-- (proiect personal, fara suport comercial)"));

    layout->addStretch();
    auto *description = new QLabel(
        QStringLiteral("Vizualizare autorizata a ecranelor din propria retea, fara control la "
                       "distanta si fara transfer de fisiere."),
        dialog);
    description->setWordWrap(true);
    description->setStyleSheet(QStringLiteral("color: #c9cdd3;"));
    layout->addWidget(description);

    auto *copyright = new QLabel(
        QStringLiteral("© %1 -- proiect personal.").arg(QDate::currentDate().year()), dialog);
    copyright->setStyleSheet(QStringLiteral("color: #6f747d; font-size: 8pt;"));
    layout->addWidget(copyright);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
    connect(buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    layout->addWidget(buttons);

    dialog->show();
}

void MainWindow::updateStaterNode(bool online)
{
    if (!m_staterNodeDot) {
        return;
    }
    m_staterNodeDot->setPixmap(QPixmap(online ? QStringLiteral(":/staterNodes/bg_empty.png")
                                              : QStringLiteral(":/staterNodes/bg_offline.png")));
    // StaterNodes.qml's tooltip: the server icon + "<name> (Online/Offline)".
    // A rich-text QLabel tooltip lets us embed the same server glyph.
    const QString server = m_staterNodeName.isEmpty() ? QStringLiteral("KikiHost") : m_staterNodeName;
    m_staterNodeDot->setToolTip(QStringLiteral(
        "<img src=':/staterNodes/server%1.png'>&nbsp;%2 (%3)")
        .arg(online ? QStringLiteral("_online") : QString(), server,
             online ? QStringLiteral("Online") : QStringLiteral("Offline")));
}

void MainWindow::updateStatus(const QString &text, bool connected)
{
    updateStaterNode(connected);
    m_statusLabel->setText(QStringLiteral("●  %1").arg(text));
    m_statusLabel->setProperty("connected", connected);
    m_statusLabel->style()->unpolish(m_statusLabel);
    m_statusLabel->style()->polish(m_statusLabel);
    m_connectButton->setText(connected ? QStringLiteral("Deconecteaza")
                                       : QStringLiteral("Conecteaza"));
    if (connected) {
        m_noConnectionTimer->stop();
        m_noEmployeesTimer->start();
    }
}

void MainWindow::setAgentIdentity(const QString &agentName, const QString &sessionName)
{
    // Used to (invisibly) set m_agentLabel's text -- that label was never
    // actually added to any layout, so this never showed anywhere. Now that
    // m_agentLabel is the real NoEmployeesAssignedInformer (see addMonitor/
    // the timers in the constructor), this identity goes in the window
    // title instead so the information isn't just dropped.
    setWindowTitle(sessionName.isEmpty()
                       ? QStringLiteral("Viewer -- %1").arg(agentName)
                       : QStringLiteral("Viewer -- %1 / %2").arg(agentName, sessionName));
    // StaterNodes.qml names the node in its tooltip -- use the agent's own
    // reported name once we have it.
    if (!agentName.isEmpty()) {
        m_staterNodeName = agentName;
        updateStaterNode(m_statusLabel && m_statusLabel->property("connected").toBool());
    }
}

QString MainWindow::deviceDisplayName(quint32 sessionKey) const
{
    // CategorizationButton's employee-rename equivalent (see
    // DeviceDetailView::renameRequested) -- a purely local override, wins
    // over whatever the agent itself reports.
    const QString renamedTo = m_deviceNameOverrides.value(sessionKey);
    if (!renamedTo.isEmpty()) {
        return renamedTo;
    }
    const QString username = m_deviceUsernames.value(sessionKey);
    if (!username.isEmpty()) {
        return username;
    }
    const quint32 primary = m_devicePrimaryStream.value(sessionKey, sessionKey);
    return m_monitorNames.value(primary, QStringLiteral("Device %1").arg(sessionKey));
}

void MainWindow::addMonitor(quint32 streamId, const QString &name, const QSize &size,
                            quint32 sessionId, const QString &sessionUsername,
                            const QString &sessionState, bool isWindow)
{
    Q_UNUSED(size)
    m_agentLabel->hide(); // a device showed up -- dismiss the informer, see the timer's comment
    if (m_monitors.contains(streamId)) {
        return;
    }
    const quint32 deviceKey = sessionId != 0 ? sessionId : streamId;
    // Stamp when this device's connection state last changed, for the
    // "online from / offline since <date>" line in the employee picker
    // (employeePicker/Header.qml's getTimestampText).
    if (!m_deviceStateSince.contains(deviceKey) || m_deviceSessionState.value(deviceKey) != sessionState) {
        m_deviceStateSince.insert(deviceKey, QDateTime::currentDateTime());
    }
    if (!sessionState.isEmpty()) {
        m_deviceSessionState.insert(deviceKey, sessionState);
        for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
            if (tile->sessionKey() == deviceKey) {
                tile->setSessionState(sessionState);
            }
        }
        if (m_openDeviceKey == deviceKey) {
            m_deviceDetailView->setSessionState(sessionState);
        }
    }
    const QString displayName = sessionUsername.isEmpty()
        ? name : QStringLiteral("%1 — %2").arg(sessionUsername, name);

    auto *monitor = new MonitorWidget(streamId, displayName, this);
    connect(monitor, &MonitorWidget::selected, this, &MainWindow::selectMonitor);
    connect(monitor, &MonitorWidget::fullScreenRequested,
            this, &MainWindow::showMonitorFullScreen);
    monitor->hide();
    m_monitors.insert(streamId, monitor);

    if (!sessionUsername.isEmpty()) {
        m_deviceUsernames.insert(deviceKey, sessionUsername);
    }

    m_streamDeviceKey.insert(streamId, deviceKey);

    if (isWindow) {
        // WindowListCapture's live preview for one open window -- not one
        // of the device's monitors (doesn't count for tile
        // thumbnail/primary-stream selection, doesn't get stacked on the
        // Monitors sub-tab); remembered separately for DeviceDetailView's
        // Programs sub-tab, removed by removeWindowStream() once
        // ViewerConnection::monitorClosed says the window actually closed
        // (see frameprotocol.h's StreamClosed -- a background window's
        // frame never updates again on its own, so "stale" can't mean
        // "closed" for these; this is the real signal instead). Also
        // offered as the tile video selector's "W" (active window) option
        // -- see refreshTileStreamsForDevice().
        m_deviceWindowStreams[deviceKey].append(streamId);
        m_windowStreamIds.insert(streamId);
        refreshTileStreamsForDevice(deviceKey);
        refreshOpenDeviceWindowPreviews(deviceKey);
        return;
    }

    m_monitorNames.insert(streamId, displayName);
    const bool isNewDevice = !m_devicePrimaryStream.contains(deviceKey);
    if (isNewDevice) {
        m_devicePrimaryStream.insert(deviceKey, streamId);
        m_connection.requestCategories(streamId);
    }
    m_deviceMonitorStreams[deviceKey].append(streamId);

    for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
        if (tile->sessionKey() == deviceKey) {
            tile->setDisplayName(deviceDisplayName(deviceKey));
        }
    }
    refreshTileStreamsForDevice(deviceKey);

    refreshHistoryDevices();
    if (m_selectedStream == 0) {
        selectMonitor(streamId);
    }
}

void MainWindow::refreshTileStreamsForDevice(quint32 deviceKey)
{
    const QList<quint32> monitorStreams = m_deviceMonitorStreams.value(deviceKey);
    const quint32 windowStream = m_deviceActiveWindowStream.value(deviceKey, 0);
    const quint32 activeMonitor = m_deviceActiveMonitorStream.value(deviceKey, 0);
    for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
        if (tile->sessionKey() == deviceKey) {
            tile->setAvailableStreams(monitorStreams, windowStream, activeMonitor);
        }
    }
}

void MainWindow::refreshOpenDeviceWindowPreviews(quint32 deviceKey)
{
    if (m_openDeviceKey != deviceKey) {
        return;
    }
    QList<MonitorWidget *> windowPreviews;
    for (quint32 streamId : m_deviceWindowStreams.value(deviceKey)) {
        if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
            windowPreviews.append(monitor);
        }
    }
    m_deviceDetailView->refreshWindowPreviews(windowPreviews);
}

void MainWindow::updateFrame(quint32 streamId, const QImage &image,
                             quint64 sequence, qint64 latencyMs)
{
    if (!m_monitors.contains(streamId)) {
        addMonitor(streamId, QStringLiteral("Monitor %1").arg(streamId), image.size(), 0,
                  QString(), QString(), false);
    }
    m_monitors.value(streamId)->setFrame(image, sequence, latencyMs);

    // Feed every tile of this stream's device -- each tile caches every
    // stream it could show (see DeviceTileWidget::updateThumbnail) and only
    // repaints if the updated stream happens to be the one it's currently
    // displaying via its video selector.
    const quint32 deviceKey = m_streamDeviceKey.value(streamId, 0);
    if (deviceKey != 0) {
        for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
            if (tile->sessionKey() == deviceKey) {
                tile->updateThumbnail(streamId, image);
            }
        }
        // A window stream that just delivered a frame is the foreground
        // window (only it gets live updates) -- that's the "active program"
        // winmode follows. Re-point the tiles' winmode target when it moves.
        if (m_windowStreamIds.contains(streamId)
            && m_deviceActiveWindowStream.value(deviceKey) != streamId) {
            m_deviceActiveWindowStream.insert(deviceKey, streamId);
            refreshTileStreamsForDevice(deviceKey);
        }
    }
}

void MainWindow::updateMetadata(quint32 streamId, const QString &application,
                                const QString &idleText, quint32 activeMonitorStreamId)
{
    if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
        monitor->setMetadata(application, idleText);
    }
    const quint32 deviceKey = m_streamDeviceKey.value(streamId, 0);
    if (deviceKey != 0) {
        // Every monitor's metadata carries the same active-monitor stream;
        // update the tiles' "Show active monitor" target when it moves.
        if (m_deviceActiveMonitorStream.value(deviceKey) != activeMonitorStreamId) {
            m_deviceActiveMonitorStream.insert(deviceKey, activeMonitorStreamId);
            refreshTileStreamsForDevice(deviceKey);
        }
        if (m_devicePrimaryStream.value(deviceKey) == streamId) {
            m_deviceActivity.insert(deviceKey, {application, idleText});
            refreshTileActivity(deviceKey);
        }
    }
}

QString MainWindow::deviceCategory(quint32 deviceKey, const QString &application) const
{
    // The employee's own rule wins over the global one (same as History).
    const QHash<QString, QString> employee = m_deviceEmployeeCategories.value(deviceKey);
    return employee.value(application, m_globalCategories.value(application));
}

void MainWindow::refreshTileActivity(quint32 deviceKey)
{
    const QPair<QString, QString> activity = m_deviceActivity.value(deviceKey);
    const QString category = deviceCategory(deviceKey, activity.first);
    for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
        if (tile->sessionKey() == deviceKey) {
            tile->setActivity(activity.first, activity.second, category);
        }
    }
}

void MainWindow::selectMonitor(quint32 streamId)
{
    m_selectedStream = streamId;
    for (auto it = m_monitors.cbegin(); it != m_monitors.cend(); ++it) {
        it.value()->setSelected(it.key() == streamId);
    }
}

void MainWindow::showMonitorFullScreen(quint32 streamId)
{
    MonitorWidget *monitor = m_monitors.value(streamId, nullptr);
    if (!monitor || monitor->currentFrame().isNull()) {
        return;
    }
    auto *window = new FullScreenFrame(monitor->currentFrame());
    window->setWindowTitle(QStringLiteral("Monitor %1 — Esc sau dublu clic pentru inchidere")
                               .arg(streamId));
    window->showFullScreen();
}

void MainWindow::showProtocolError(const QString &message)
{
    statusBar()->showMessage(message, 8000);
}

void MainWindow::showTrackerPage()
{
    m_trackerNavButton->setActivated(true);
    m_historyNavButton->setActivated(false);
    m_deviceDetailView->deactivate();
    m_contentStack->setCurrentWidget(m_trackerPage);
    m_subbar->show();
}

void MainWindow::showHistoryPage()
{
    m_subbar->hide();
    m_historyNavButton->setActivated(true);
    m_trackerNavButton->setActivated(false);
    m_deviceDetailView->deactivate();
    m_contentStack->setCurrentWidget(m_historyView);
    m_historyView->activate();
}

void MainWindow::switchTab(int index)
{
    if (index < 0 || index >= m_tabs.size()) {
        return;
    }
    m_currentTabIndex = index;
    relayoutCurrentTab();
}

void MainWindow::addNewTab()
{
    // DialogWithBipanelEmployeeSelector.qml ("Add department"): pick a name
    // and any number of employees at once (left = available, right =
    // selected), then create the tab pre-filled with them. We have no
    // department model, so the left side is a single "Conection List" group
    // of the currently connected devices (as the screenshot shows without
    // departments).
    auto *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(QStringLiteral("Add department"));
    dialog->setModal(true);
    dialog->setMinimumSize(527, 500);
    auto *layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(20, 20, 20, 16);
    layout->setSpacing(10);

    auto *nameRow = new QHBoxLayout;
    auto *nameLabel = new QLabel(QStringLiteral("Department name"), dialog);
    nameLabel->setStyleSheet(QStringLiteral("color: white; font-weight: 600;"));
    auto *nameEdit = new QLineEdit(dialog);
    nameEdit->setPlaceholderText(QStringLiteral("New tab"));
    nameRow->addWidget(nameLabel);
    nameRow->addWidget(nameEdit, 1);
    layout->addLayout(nameRow);

    auto *searchRow = new QHBoxLayout;
    auto *searchLabel = new QLabel(QStringLiteral("Add employee"), dialog);
    searchLabel->setStyleSheet(QStringLiteral("color: white; font-weight: 600;"));
    auto *searchEdit = new QLineEdit(dialog);
    searchEdit->setPlaceholderText(QStringLiteral("Enter employee name"));
    searchRow->addWidget(searchLabel);
    searchRow->addWidget(searchEdit, 1);
    layout->addLayout(searchRow);

    auto *onlineOnlyCheck = new QCheckBox(QStringLiteral("Show online employees only"), dialog);
    layout->addWidget(onlineOnlyCheck);

    auto *countLabel = new QLabel(dialog);
    countLabel->setStyleSheet(QStringLiteral(
        "color: white; background: #3a3d45; border: 1px solid #4d515b; padding: 4px 8px;"));
    layout->addWidget(countLabel, 0, Qt::AlignLeft);

    // Bipanel: left = available (a "Conection List" group of devices), right
    // = selected. Clicking a row moves it across.
    auto *panels = new QHBoxLayout;
    panels->setSpacing(10);
    auto *available = new QTreeWidget(dialog);
    available->setHeaderHidden(true);
    available->setRootIsDecorated(false);
    available->setIndentation(14);
    available->setItemDelegate(new EmployeeRowDelegate(available));
    auto *selected = new QTreeWidget(dialog);
    selected->setHeaderHidden(true);
    selected->setRootIsDecorated(false);
    selected->setIndentation(0);
    selected->setItemDelegate(new EmployeeRowDelegate(selected));
    panels->addWidget(available, 1);
    panels->addWidget(selected, 1);
    layout->addLayout(panels, 1);

    // "Conection List" group header (green, non-selectable).
    auto *group = new QTreeWidgetItem(available, {QStringLiteral("Conection List")});
    group->setFlags(Qt::ItemIsEnabled);
    group->setForeground(0, QColor(0x28, 0xa3, 0x6f));
    QFont groupFont = group->font(0);
    groupFont.setBold(true);
    group->setFont(0, groupFont);
    available->expandAll();

    const auto makeDeviceItem = [this](quint32 deviceKey) {
        auto *item = new QTreeWidgetItem({deviceDisplayName(deviceKey)});
        const bool offline = m_deviceSessionState.value(deviceKey) == QStringLiteral("disconnected");
        item->setData(0, EmployeeRowDelegate::kOfflineRole, offline);
        const QDateTime since = m_deviceStateSince.value(deviceKey);
        if (since.isValid()) {
            const QString when = since.toString(QStringLiteral("M/d/yyyy HH:mm"));
            item->setData(0, EmployeeRowDelegate::kTimestampRole,
                          offline ? QStringLiteral("offline since %1").arg(when)
                                  : QStringLiteral("online from %1").arg(when));
        }
        item->setData(0, EmployeeRowDelegate::kDeviceKeyRole, deviceKey);
        return item;
    };
    for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
        group->addChild(makeDeviceItem(it.key()));
    }

    auto updateCount = [countLabel, selected] {
        countLabel->setText(QStringLiteral("Selected %1/%2")
                                .arg(selected->topLevelItemCount())
                                .arg(kMaxTilesPerTab));
    };
    updateCount();

    auto applyFilter = [group, available, onlineOnlyCheck, searchEdit] {
        const QString needle = searchEdit->text().trimmed();
        const bool onlineOnly = onlineOnlyCheck->isChecked();
        for (int i = 0; i < group->childCount(); ++i) {
            QTreeWidgetItem *child = group->child(i);
            const bool offline = child->data(0, EmployeeRowDelegate::kOfflineRole).toBool();
            const bool okSearch = needle.isEmpty() || child->text(0).contains(needle, Qt::CaseInsensitive);
            child->setHidden(!(okSearch && (!onlineOnly || !offline)));
        }
        Q_UNUSED(available);
    };
    connect(searchEdit, &QLineEdit::textChanged, dialog, applyFilter);
    connect(onlineOnlyCheck, &QCheckBox::toggled, dialog, applyFilter);

    // Move a device to the selected side (respecting the 25 cap).
    connect(available, &QTreeWidget::itemClicked, dialog,
            [=](QTreeWidgetItem *item, int) {
                if (!item || item == group || item->isHidden()) {
                    return;
                }
                if (selected->topLevelItemCount() >= kMaxTilesPerTab) {
                    return;
                }
                const quint32 deviceKey = item->data(0, EmployeeRowDelegate::kDeviceKeyRole).toUInt();
                delete item; // remove from the available group
                selected->addTopLevelItem(makeDeviceItem(deviceKey));
                updateCount();
            });
    // Click a selected row to put it back.
    connect(selected, &QTreeWidget::itemClicked, dialog,
            [=](QTreeWidgetItem *item, int) {
                if (!item) {
                    return;
                }
                const quint32 deviceKey = item->data(0, EmployeeRowDelegate::kDeviceKeyRole).toUInt();
                delete item;
                group->addChild(makeDeviceItem(deviceKey));
                applyFilter();
                updateCount();
            });

    auto *buttonRow = new QHBoxLayout;
    buttonRow->addStretch(1);
    auto *cancelButton = new QPushButton(QStringLiteral("Cancel"), dialog);
    auto *okButton = new QPushButton(QStringLiteral("OK"), dialog);
    okButton->setObjectName(QStringLiteral("primaryButton"));
    buttonRow->addWidget(cancelButton);
    buttonRow->addWidget(okButton);
    layout->addLayout(buttonRow);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::close);
    connect(okButton, &QPushButton::clicked, dialog, [=] {
        QList<quint32> deviceKeys;
        for (int i = 0; i < selected->topLevelItemCount(); ++i) {
            deviceKeys.append(selected->topLevelItem(i)->data(0, EmployeeRowDelegate::kDeviceKeyRole).toUInt());
        }
        const QString title = nameEdit->text().trimmed().isEmpty()
            ? QStringLiteral("New tab_%1").arg(m_tabs.size() + 1)
            : nameEdit->text().trimmed();
        createTabWithDevices(title, deviceKeys);
        dialog->close();
    });

    dialog->show();
}

void MainWindow::createTabWithDevices(const QString &title, const QList<quint32> &deviceKeys)
{
    // TrackerQuadrator.qml's enplaceFullCells: grid shape is derived once from
    // the employee count (columns = max(ceil(sqrt(n)), 2), rows = max(ceil(n/
    // columns), 2)), then every employee gets a cell.
    TrackerTab tab;
    tab.title = title;
    for (quint32 deviceKey : deviceKeys) {
        tab.cells.append({m_nextTileId++, deviceKey});
    }
    const int n = deviceKeys.size();
    const int columns = qMax(static_cast<int>(std::ceil(std::sqrt(static_cast<double>(n)))), 2);
    tab.gridColumns = columns;
    tab.gridRows = qMax(n > 0 ? qCeil(static_cast<double>(n) / columns) : 0, 2);
    m_tabs.append(tab);
    m_tabBar->addTab(QStringLiteral("• %1").arg(title));
    m_tabBar->setCurrentIndex(m_tabs.size() - 1);
}

void MainWindow::closeTab(int index)
{
    if (m_tabs.size() <= 1 || index < 0 || index >= m_tabs.size()) {
        return; // always keep at least one tab
    }
    m_tabs.removeAt(index);
    m_tabBar->removeTab(index);
    if (m_currentTabIndex >= m_tabs.size()) {
        m_currentTabIndex = m_tabs.size() - 1;
    }
    relayoutCurrentTab();
}

void MainWindow::moveTab(int from, int to)
{
    if (from < 0 || to < 0 || from >= m_tabs.size() || to >= m_tabs.size() || from == to) {
        return;
    }
    m_tabs.move(from, to);
    if (m_currentTabIndex == from) {
        m_currentTabIndex = to;
    } else if (from < m_currentTabIndex && m_currentTabIndex <= to) {
        --m_currentTabIndex;
    } else if (to <= m_currentTabIndex && m_currentTabIndex < from) {
        ++m_currentTabIndex;
    }
}

void MainWindow::renameTab(int index)
{
    if (index < 0 || index >= m_tabs.size()) {
        return;
    }
    bool accepted = false;
    const QString title = QInputDialog::getText(
        this, QStringLiteral("Redenumeste tab"), QStringLiteral("Nume tab:"),
        QLineEdit::Normal, m_tabs.at(index).title, &accepted).trimmed();
    if (!accepted || title.isEmpty()) {
        return;
    }
    m_tabs[index].title = title;
    m_tabBar->setTabText(index, QStringLiteral("• %1").arg(title));
}

void MainWindow::toggleFiltersPanel(bool visible)
{
    if (m_filtersPanel) {
        m_filtersPanel->setVisible(visible);
    }
}

void MainWindow::setSimpleMode(bool simple)
{
    m_simpleMode = simple;
    saveSettings();
}

void MainWindow::openGridsPanel()
{
    if (m_tabs.isEmpty()) {
        return;
    }
    const TrackerTab &tab = m_tabs[m_currentTabIndex];
    auto *panel = new GridsPanel(tab.gridLayout, tab.gridLayoutVertical, this);
    connect(panel, &GridsPanel::layoutChosen, this, [this](const QString &layout, bool vertical) {
        if (!m_tabs.isEmpty()) {
            m_tabs[m_currentTabIndex].gridLayout = layout;
            m_tabs[m_currentTabIndex].gridLayoutVertical = vertical;
            relayoutCurrentTab();
        }
    });

    auto *sender = qobject_cast<QWidget *>(this->sender());
    const QPoint popupPos = sender ? sender->mapToGlobal(QPoint(0, sender->height())) : QCursor::pos();
    panel->move(popupPos);
    panel->show();
}

void MainWindow::openAddDeviceDialog(const GridFillTarget &target)
{
    // A device can be added to a tab more than once (e.g. to watch two of
    // its monitors side by side), so the list below is never filtered by
    // what's already in the tab. The tab's total tile count is capped at
    // kMaxTilesPerTab (matches the real Kickidler viewer); relayoutCurrentTab
    // already disables the "+" tile itself once that's hit, so this is just
    // a defensive second guard (e.g. against a stray call).
    if (!m_tabs.isEmpty() && m_tabs[m_currentTabIndex].tileCount() >= kMaxTilesPerTab) {
        statusBar()->showMessage(
            QStringLiteral("Limita de device-uri pe acest tab a fost atinsa (25)."), 4000);
        return;
    }

    // Only one picker at a time: clicking a "+" while the window is already
    // open just brings it back to the front instead of stacking a second
    // copy (the real Kickidler viewer is single-instance here too).
    if (m_addDeviceDialog) {
        m_addDeviceDialog->show();
        m_addDeviceDialog->raise();
        m_addDeviceDialog->activateWindow();
        return;
    }

    auto *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    m_addDeviceDialog = dialog;
    dialog->setWindowTitle(QStringLiteral("Select an Employee"));
    dialog->setModal(false);
    dialog->setMinimumSize(420, 440);
    auto *layout = new QVBoxLayout(dialog);

    // utils/EmployeePicker.qml equivalent: real one is a full Organization ->
    // Department -> Employee -> Session tree with search and an "online
    // only" filter. We have no department/organization model (KikiHost
    // just reports flat connected sessions), so this is a single-level tree
    // under one "Toate device-urile" root instead of real departments --
    // but search and the online filter are real and wired against actual
    // session state (m_deviceSessionState), not stubs.
    auto *searchEdit = new QLineEdit(dialog);
    searchEdit->setPlaceholderText(QStringLiteral("Type name or login"));
    layout->addWidget(searchEdit);

    auto *onlineOnlyCheck = new QCheckBox(QStringLiteral("Show online employees only"), dialog);
    layout->addWidget(onlineOnlyCheck);

    auto *tree = new QTreeWidget(dialog);
    tree->setHeaderHidden(true);
    tree->setRootIsDecorated(!m_simpleMode);
    tree->setIndentation(m_simpleMode ? 0 : 16);
    tree->setMouseTracking(true);
    tree->setItemDelegate(new EmployeeRowDelegate(tree));
    // Simple mode hides the department-shaped root label entirely (there's
    // no department model behind it anyway); Advanced mode shows it as a
    // placeholder for where real departments would nest once KikiHost
    // has an org model.
    QTreeWidgetItem *root = m_simpleMode ? tree->invisibleRootItem()
                                         : new QTreeWidgetItem(tree, {QStringLiteral("Toate device-urile")});
    if (!m_simpleMode) {
        // The QTreeWidgetItem(tree, ...) constructor above already appends
        // it as a top-level item -- root is just made non-selectable here.
        root->setFlags(root->flags() & ~Qt::ItemIsSelectable);
    }

    QHash<QTreeWidgetItem *, quint32> deviceForItem;
    for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
        const quint32 deviceKey = it.key();
        auto *deviceItem = new QTreeWidgetItem(root, {deviceDisplayName(deviceKey)});
        const bool offline = m_deviceSessionState.value(deviceKey) == QStringLiteral("disconnected");
        deviceItem->setData(0, EmployeeRowDelegate::kOfflineRole, offline);
        // "online from / offline since <date>" (employeePicker getTimestampText).
        const QDateTime since = m_deviceStateSince.value(deviceKey);
        QString timestamp;
        if (since.isValid()) {
            const QString when = since.toString(QStringLiteral("M/d/yyyy HH:mm"));
            timestamp = offline ? QStringLiteral("offline since %1").arg(when)
                                : QStringLiteral("online from %1").arg(when);
        } else {
            timestamp = offline ? QStringLiteral("Offline") : QString();
        }
        deviceItem->setData(0, EmployeeRowDelegate::kTimestampRole, timestamp);
        deviceForItem.insert(deviceItem, deviceKey);
    }
    tree->expandAll();
    if (deviceForItem.isEmpty()) {
        auto *empty = new QLabel(
            QStringLiteral("Niciun device disponibil -- doar cele conectate acum pot fi adaugate."),
            dialog);
        empty->setWordWrap(true);
        layout->addWidget(empty);
    } else {
        layout->addWidget(tree, 1);
    }

    auto applyFilter = [tree, root, onlineOnlyCheck, searchEdit]() {
        const QString needle = searchEdit->text().trimmed();
        const bool onlineOnly = onlineOnlyCheck->isChecked();
        for (int i = 0; i < root->childCount(); ++i) {
            QTreeWidgetItem *child = root->child(i);
            const bool offline = child->data(0, Qt::UserRole).toBool();
            const bool matchesSearch =
                needle.isEmpty() || child->text(0).contains(needle, Qt::CaseInsensitive);
            const bool matchesOnline = !onlineOnly || !offline;
            child->setHidden(!(matchesSearch && matchesOnline));
        }
    };
    connect(searchEdit, &QLineEdit::textChanged, dialog, applyFilter);
    connect(onlineOnlyCheck, &QCheckBox::toggled, dialog, applyFilter);

    // EmployeeSelector.qml: Cancel + a green "Ok". Selection first, then Ok
    // confirms (Ok is disabled until an employee row is selected).
    auto *buttonRow = new QHBoxLayout;
    buttonRow->addStretch(1);
    auto *cancelButton = new QPushButton(QStringLiteral("Cancel"), dialog);
    auto *okButton = new QPushButton(QStringLiteral("Ok"), dialog);
    okButton->setObjectName(QStringLiteral("primaryButton")); // green, see applyStyle()
    okButton->setEnabled(false);
    buttonRow->addWidget(cancelButton);
    buttonRow->addWidget(okButton);
    layout->addLayout(buttonRow);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::close);

    // Shared confirm: fill the target cell with the selected device, honoring
    // the non-modal guards (tab switched / cell already filled meanwhile).
    auto confirm = [this, dialog, deviceForItem, target, tabIndex = m_currentTabIndex](QTreeWidgetItem *item) {
        if (!item || !deviceForItem.contains(item)) {
            return;
        }
        dialog->close();
        if (tabIndex != m_currentTabIndex || m_tabs.isEmpty()) {
            return;
        }
        const TrackerTab &currentTab = m_tabs.at(m_currentTabIndex);
        const bool isFictive = target.fictiveRow >= 0 || target.fictiveColumn >= 0;
        if (!isFictive && target.indexInGrid >= 0 && target.indexInGrid < currentTab.cells.size()
            && currentTab.cells.at(target.indexInGrid).tileId != 0) {
            return;
        }
        fillTarget(target, deviceForItem.value(item));
    };

    // Single click selects (enables Ok); double click confirms directly.
    connect(tree, &QTreeWidget::itemSelectionChanged, dialog, [tree, okButton, deviceForItem] {
        const QList<QTreeWidgetItem *> selected = tree->selectedItems();
        okButton->setEnabled(!selected.isEmpty() && deviceForItem.contains(selected.first()));
    });
    connect(okButton, &QPushButton::clicked, dialog, [tree, confirm] {
        const QList<QTreeWidgetItem *> selected = tree->selectedItems();
        confirm(selected.isEmpty() ? nullptr : selected.first());
    });
    connect(tree, &QTreeWidget::itemDoubleClicked, dialog,
            [confirm](QTreeWidgetItem *item, int) { confirm(item); });

    dialog->show();
}

void MainWindow::openDeviceDetail(quint32 sessionKey)
{
    const quint32 primaryStream = m_devicePrimaryStream.value(sessionKey, 0);
    if (primaryStream == 0) {
        return;
    }
    QList<MonitorWidget *> monitors;
    for (quint32 streamId : m_deviceMonitorStreams.value(sessionKey)) {
        if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
            monitors.append(monitor);
        }
    }
    QList<MonitorWidget *> windowPreviews;
    for (quint32 streamId : m_deviceWindowStreams.value(sessionKey)) {
        if (MonitorWidget *monitor = m_monitors.value(streamId, nullptr)) {
            windowPreviews.append(monitor);
        }
    }
    m_openDeviceKey = sessionKey;
    m_deviceDetailView->setSessionState(m_deviceSessionState.value(sessionKey));
    m_deviceDetailView->showDevice(sessionKey, deviceDisplayName(sessionKey), primaryStream, monitors,
                                   windowPreviews);
    m_contentStack->setCurrentWidget(m_deviceDetailView);
    m_deviceDetailView->activate();
    m_subbar->hide();
}

void MainWindow::closeDeviceDetail()
{
    m_deviceDetailView->deactivate();
    m_contentStack->setCurrentWidget(m_trackerPage);
    m_subbar->show();
    m_openDeviceKey = 0;
}

void MainWindow::removeWindowStream(quint32 streamId)
{
    // ViewerConnection::monitorClosed -- the authoritative "this window
    // actually closed" signal (see frameprotocol.h's StreamClosed). Only
    // ever fires for window streams (real monitors don't come and go), but
    // guard anyway in case that ever changes.
    if (!m_windowStreamIds.remove(streamId)) {
        return;
    }
    const quint32 deviceKey = m_streamDeviceKey.take(streamId);
    if (deviceKey != 0) {
        m_deviceWindowStreams[deviceKey].removeOne(streamId);
        // If the active (foreground) window itself closed, forget it -- the
        // next window frame to arrive re-establishes which one is active.
        if (m_deviceActiveWindowStream.value(deviceKey) == streamId) {
            m_deviceActiveWindowStream.remove(deviceKey);
        }
    }
    if (MonitorWidget *monitor = m_monitors.take(streamId)) {
        // refreshOpenDeviceWindowPreviews() below (if it applies to this
        // device) rebuilds and re-lays-out from m_deviceWindowStreams --
        // already updated above -- before this deleteLater() actually
        // runs, so it never touches the stale pointer.
        monitor->deleteLater();
    }
    if (deviceKey != 0) {
        refreshTileStreamsForDevice(deviceKey);
        refreshOpenDeviceWindowPreviews(deviceKey);
    }
}

void MainWindow::buildInterface()
{
    setWindowTitle(QStringLiteral("Viewer"));
    // Tall enough that History's controls block (buttons/timeline/toggle/
    // Activity-Efficiency) isn't clipped at the bottom by default -- see
    // HistoryView's m_videoColumn maximumHeight comment.
    resize(1500, 960);
    setMinimumSize(920, 560);

    auto *root = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(root);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto *header = new QFrame(root);
    header->setObjectName(QStringLiteral("header"));
    header->setFixedHeight(42);
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 8, 0);
    headerLayout->setSpacing(0);

    auto *menuButton = new QToolButton(header);
    menuButton->setObjectName(QStringLiteral("menuButton"));
    menuButton->setText(QStringLiteral("☰"));
    menuButton->setFixedSize(44, 42);
    menuButton->setPopupMode(QToolButton::InstantPopup);
    auto *menu = new QMenu(menuButton);
    // Real Kickidler viewer's hamburger menu has "Settings" open a
    // Language/Font scale/Tooltips dialog (Settings.qml) -- our app also
    // needs a host/port/token to even connect, a concept the original
    // (cloud-account based) doesn't have, so that gets its own clearly
    // labeled "Connection..." entry instead of overloading "Settings".
    QAction *preferencesAction = menu->addAction(QStringLiteral("Settings"));
    QAction *connectionAction = menu->addAction(QStringLiteral("Connection..."));
    QAction *clearAction = menu->addAction(QStringLiteral("Delete local cache on Viewer restart"));
    menu->addSeparator();
    // TopPanel.qml's "Simple/Advanced mode" switch -- the real one swaps
    // the whole organization model (departments/roles disappear in Simple
    // mode). We have no department model to swap (see EmployeePicker gap),
    // so this only tells the Grids panel's "assign department" entry and
    // the add-device picker's tree root to hide -- everything Advanced mode
    // would otherwise show that we don't have yet.
    QAction *basicAction = menu->addAction(QStringLiteral("Basic version (Simple mode)"));
    basicAction->setCheckable(true);
    basicAction->setChecked(m_simpleMode);
    m_simpleModeAction = basicAction;
    QAction *aboutAction = menu->addAction(QStringLiteral("About the program"));
    QAction *exitAction = menu->addAction(QStringLiteral("Exit"));
    menuButton->setMenu(menu);
    connect(preferencesAction, &QAction::triggered, this, &MainWindow::showPreferences);
    connect(connectionAction, &QAction::triggered, this, &MainWindow::showSettings);
    connect(clearAction, &QAction::triggered, this, [this]() {
        // Matches the action's own label ("...on Viewer restart") and the
        // real app.scheduleLocalCacheRemoval(): marks the cache for
        // deletion on the NEXT launch instead of wiping it out from under
        // the still-running app, plus a confirmation message -- neither of
        // which the previous QSettings().clear() (immediate, silent) did.
        QFile marker(pendingCacheClearMarkerPath());
        (void)marker.open(QIODevice::WriteOnly);
        QMessageBox::information(
            this, QStringLiteral("Delete local cache"),
            QStringLiteral("Cache-ul local va fi sters la urmatoarea pornire a Viewer-ului."));
    });
    connect(basicAction, &QAction::toggled, this, &MainWindow::setSimpleMode);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    auto *brand = new QLabel(QStringLiteral("Kiki"), header);
    brand->setObjectName(QStringLiteral("brand"));
    brand->setFixedWidth(165);
    headerLayout->addWidget(menuButton);
    headerLayout->addWidget(brand);
    headerLayout->addStretch();

    // TopPanel.qml's primaryActivities row: Tracker / Reports / History /
    // Remote Control, 25px apart. Reports and Remote Control have no
    // backing feature in this system (no reporting engine, no remote
    // control -- AgentSettings explicitly refuses that capability), so
    // they're shown for visual parity but disabled rather than silently
    // inert.
    m_trackerNavButton = new TopNavButton(QStringLiteral("tracker"), QStringLiteral("Tracker"), header);
    m_trackerNavButton->setActivated(true);
    connect(m_trackerNavButton, &QPushButton::clicked, this, &MainWindow::showTrackerPage);
    m_reportsNavButton = new TopNavButton(QStringLiteral("reports"), QStringLiteral("Reports"), header);
    m_reportsNavButton->setEnabled(false);
    m_reportsNavButton->setToolTip(QStringLiteral("Nu exista un motor de rapoarte in acest sistem."));
    m_historyNavButton = new TopNavButton(QStringLiteral("history"), QStringLiteral("History"), header);
    connect(m_historyNavButton, &QPushButton::clicked, this, &MainWindow::showHistoryPage);
    m_remoteControlNavButton = new TopNavButton(QStringLiteral("support"), QStringLiteral("Remote Control"), header);
    m_remoteControlNavButton->setEnabled(false);
    m_remoteControlNavButton->setToolTip(
        QStringLiteral("Controlul de la distanta nu este permis in acest sistem."));
    headerLayout->addWidget(m_trackerNavButton);
    headerLayout->addSpacing(25);
    headerLayout->addWidget(m_reportsNavButton);
    headerLayout->addSpacing(25);
    headerLayout->addWidget(m_historyNavButton);
    headerLayout->addSpacing(25);
    headerLayout->addWidget(m_remoteControlNavButton);
    headerLayout->addStretch();

    // TopPanel.qml: Image{line.png} Image{line_1.png} Item{5} StaterNodes
    // Item{5} Image{line.png} -- two adjacent 1px separators (a subtle
    // darker-green groove, not black) before StaterNodes, one after.
    const auto makeHeaderLine = [header](const char *asset) {
        auto *line = new QLabel(header);
        line->setPixmap(QPixmap(QStringLiteral(":/topPanel/%1").arg(QLatin1String(asset))));
        line->setScaledContents(true);
        line->setFixedSize(1, header->height());
        return line;
    };
    headerLayout->addWidget(makeHeaderLine("line.png"), 0, Qt::AlignVCenter);
    headerLayout->addWidget(makeHeaderLine("line_1.png"), 0, Qt::AlignVCenter);
    headerLayout->addSpacing(5);
    // ViewerControls/StaterNodes.qml: the server-status indicator(s) in the
    // top panel -- one dot per central node in the real (multi-node)
    // Kickidler. This system has exactly one node (KikiHost), so it's a
    // single dot: staterNodes/bg_empty.png (green) when connected,
    // bg_offline.png (red) otherwise, with a hover tooltip naming the server
    // and its state.
    m_staterNodeDot = new QLabel(header);
    m_staterNodeDot->setObjectName(QStringLiteral("staterNode"));
    m_staterNodeDot->setFixedSize(16, 16);
    // The global QWidget stylesheet gives every plain QLabel an opaque
    // #30323a background; bg_empty/bg_offline.png don't fill their full
    // 16x16 canvas, so without this the dot sits in a visible dark square.
    m_staterNodeDot->setStyleSheet(QStringLiteral("background: transparent;"));
    headerLayout->addWidget(m_staterNodeDot, 0, Qt::AlignVCenter);
    headerLayout->addSpacing(5);
    headerLayout->addWidget(makeHeaderLine("line.png"), 0, Qt::AlignVCenter);
    headerLayout->addSpacing(10);
    updateStaterNode(false);

    // TopPanel.qml's header has no status text or connect/disconnect
    // button at all -- StaterNodes above is the only connection indicator
    // (a dot + tooltip). Live status text and the Connect/Disconnect action
    // now live in the Connection settings dialog instead (see
    // buildInterface()'s m_settingsDialog and updateStatus()).
    // TopPanel.qml's maximize/minimize button (topPanel/b_maximize_*.png /
    // b_minimize_*.png, 38x38) -- real icons, not a Unicode glyph (which
    // rendered as broken corner brackets, not an actual expand icon).
    auto *fullScreenButton = new QToolButton(header);
    fullScreenButton->setObjectName(QStringLiteral("headerIcon"));
    fullScreenButton->setIconSize(QSize(38, 38));
    fullScreenButton->setAutoRaise(true);
    const auto updateFullScreenIcon = [this, fullScreenButton] {
        fullScreenButton->setIcon(QIcon(isFullScreen()
            ? QStringLiteral(":/topPanel/b_minimize_normal.png")
            : QStringLiteral(":/topPanel/b_maximize_normal.png")));
    };
    updateFullScreenIcon();
    connect(fullScreenButton, &QToolButton::clicked, this, [this, updateFullScreenIcon] {
        isFullScreen() ? showNormal() : showFullScreen();
        updateFullScreenIcon();
    });
    headerLayout->addWidget(fullScreenButton);
    rootLayout->addWidget(header);

    m_subbar = new QFrame(root);
    QFrame *subbar = m_subbar;
    subbar->setObjectName(QStringLiteral("subbar"));
    // TrackerPanel.qml: 37px, 9px in, then Grids / 9px / Filters (the
    // trackerPanel/b_*.png icon + text, #a2a2a4 -> white on hover).
    subbar->setFixedHeight(37);
    auto *subLayout = new QHBoxLayout(subbar);
    subLayout->setContentsMargins(9, 0, 8, 0);
    subLayout->setSpacing(9);
    auto *grids = new TrackerPanelButton(QStringLiteral("grid"), QStringLiteral("Grids"), subbar);
    connect(grids, &QPushButton::clicked, this, &MainWindow::openGridsPanel);
    m_filtersButton = new TrackerPanelButton(QStringLiteral("filters"), QStringLiteral("Filters"), subbar);
    m_filtersButton->setCheckable(true);
    connect(m_filtersButton, &QPushButton::toggled, this, &MainWindow::toggleFiltersPanel);
    subLayout->addWidget(grids);
    subLayout->addWidget(m_filtersButton);
    subLayout->addStretch();
    auto *plusButton = new QToolButton(subbar);
    plusButton->setObjectName(QStringLiteral("plus"));
    plusButton->setText(QStringLiteral("+"));
    connect(plusButton, &QToolButton::clicked, this, &MainWindow::addNewTab);
    subLayout->addWidget(plusButton);
    m_tabBar = new QTabBar(subbar);
    m_tabBar->setObjectName(QStringLiteral("tabs"));
    m_tabBar->setExpanding(false);
    m_tabBar->setTabsClosable(true);
    // Matches the real Kickidler viewer's ViewerControls/Tabs.qml: tabs can
    // be dragged to reorder, and double-clicking the current tab renames it.
    m_tabBar->setMovable(true);
    m_tabBar->addTab(QStringLiteral("• %1").arg(m_tabs.first().title));
    connect(m_tabBar, &QTabBar::currentChanged, this, &MainWindow::switchTab);
    connect(m_tabBar, &QTabBar::tabCloseRequested, this, &MainWindow::closeTab);
    connect(m_tabBar, &QTabBar::tabMoved, this, &MainWindow::moveTab);
    connect(m_tabBar, &QTabBar::tabBarDoubleClicked, this, &MainWindow::renameTab);
    subLayout->addWidget(m_tabBar);
    rootLayout->addWidget(subbar);

    m_trackerPage = new QWidget(root);
    auto *contentLayout = new QVBoxLayout(m_trackerPage);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    // utils/NoEmployeesAssignedInformer.qml equivalent: was created but
    // never actually put into a layout (a dead leftover, never visible) --
    // repurposed here as the real informer, shown by
    // showNoEmployeesInformerIfStillEmpty() 5s after connecting if still
    // no devices have shown up.
    m_agentLabel = new QLabel(
        QStringLiteral("Niciun device disponibil inca -- asteapta ca agentul sa se conecteze."),
        m_trackerPage);
    m_agentLabel->setObjectName(QStringLiteral("privacy"));
    m_agentLabel->setWordWrap(true);
    m_agentLabel->hide();
    contentLayout->addWidget(m_agentLabel);

    auto *contentRow = new QHBoxLayout;
    contentRow->setContentsMargins(0, 0, 0, 0);
    contentRow->setSpacing(0);

    // TrackerDefaultPrompt.qml equivalent: swaps in for the grid once the
    // current tab has no real tiles at all (an empty grid full of nothing
    // but "+" tiles otherwise, which is a valid but less clear substitute).
    m_gridStack = new QStackedWidget(m_trackerPage);
    m_gridScrollPage = new QWidget(m_gridStack);
    // TrackerQuadratorGrid.qml: a grayHatching Background fills the whole
    // grid item, behind the shadow strip and the cells -- shows through the
    // 3px cell spacing and the scroll margins.
    m_gridScrollPage->setObjectName(QStringLiteral("gridScrollPage"));
    m_gridScrollPage->setAttribute(Qt::WA_StyledBackground);
    m_gridScrollPage->setStyleSheet(QStringLiteral(
        "QWidget#gridScrollPage { background-image: url(:/activeCell/grayHatching.png); background-repeat: repeat; }"));
    // TrackerQuadratorGrid.qml: the ScrollView sits 4/4/4/3 (left/top/
    // right/bottom) inside the hatched background; the mainbgshaddow.png
    // strip is drawn OVER the top edge (an overlay Image, not a row that
    // takes layout space).
    auto *gridScrollLayout = new QVBoxLayout(m_gridScrollPage);
    gridScrollLayout->setContentsMargins(4, 4, 4, 3);
    gridScrollLayout->setSpacing(0);
    m_gridScroll = new QScrollArea(m_gridScrollPage);
    // Content size is set by relayoutCurrentTab() (QuadratorGrid.qml sizes
    // cells from the ScrollView's own size, not from the viewport).
    m_gridScroll->setWidgetResizable(false);
    m_gridScroll->setFrameShape(QFrame::NoFrame);
    m_gridScroll->setObjectName(QStringLiteral("gridScroll"));
    // Controls/ScrollView.qml: a 5px handle inside vscroll margins 4/1/3/2
    // (left/top/right/bottom) -> 12px wide bar; hscroll margins 2/4/0/3 ->
    // 12px high bar. These are the vscrollWidth/hscrollHeight the cell sizing
    // in relayoutCurrentTab() reserves.
    m_gridScroll->setStyleSheet(QStringLiteral(
        "QScrollArea#gridScroll { background: transparent; }"
        "QScrollBar:vertical { background: transparent; width: 12px; margin: 1px 3px 2px 4px; }"
        "QScrollBar::handle:vertical { background: #5a5b63; border-radius: 2px; min-height: 30px; }"
        "QScrollBar:horizontal { background: transparent; height: 12px; margin: 4px 0px 3px 2px; }"
        "QScrollBar::handle:horizontal { background: #5a5b63; border-radius: 2px; min-width: 30px; }"
        "QScrollBar::add-line, QScrollBar::sub-line { width: 0; height: 0; }"
        "QScrollBar::add-page, QScrollBar::sub-page { background: none; }"));
    m_gridScroll->viewport()->setStyleSheet(QStringLiteral("background: transparent;"));
    m_monitorContainer = new QWidget(m_gridScroll);
    m_monitorContainer->setObjectName(QStringLiteral("monitorArea"));
    m_monitorContainer->setStyleSheet(QStringLiteral("QWidget#monitorArea { background: transparent; }"));
    m_gridScroll->setWidget(m_monitorContainer);
    gridScrollLayout->addWidget(m_gridScroll, 1);
    auto *gridShadow = new QWidget(m_gridScrollPage);
    gridShadow->setObjectName(QStringLiteral("gridShadow"));
    gridShadow->setAttribute(Qt::WA_StyledBackground);
    gridShadow->setAttribute(Qt::WA_TransparentForMouseEvents);
    gridShadow->setStyleSheet(QStringLiteral(
        "QWidget#gridShadow { background-image: url(:/tracker/grid/mainbgshaddow.png); background-repeat: repeat-x; }"));
    const QSize shadowSize = QPixmap(QStringLiteral(":/tracker/grid/mainbgshaddow.png")).size();
    gridShadow->setFixedHeight(qMax(1, shadowSize.height()));
    gridShadow->raise(); // kept full-width by eventFilter()
    m_gridStack->addWidget(m_gridScrollPage);

    contentRow->addWidget(m_gridStack, 1);

    // TrackerFiltersPanel.qml equivalent -- see toggleFiltersPanel().
    m_filtersPanel = new QFrame(m_trackerPage);
    m_filtersPanel->setObjectName(QStringLiteral("filtersPanel"));
    m_filtersPanel->setFixedWidth(168);
    m_filtersPanel->hide();
    auto *filtersLayout = new QVBoxLayout(m_filtersPanel);
    filtersLayout->setContentsMargins(10, 10, 10, 10);
    auto *filtersTitle = new QLabel(QStringLiteral("Filtre"), m_filtersPanel);
    filtersTitle->setStyleSheet(QStringLiteral("color: white; font-weight: 600;"));
    filtersLayout->addWidget(filtersTitle);
    auto *filtersTree = new QTreeWidget(m_filtersPanel);
    filtersTree->setHeaderHidden(true);
    filtersTree->setDragEnabled(true);
    auto *appsGroup = new QTreeWidgetItem(filtersTree, {QStringLiteral("Aplicatii neproductive")});
    Q_UNUSED(appsGroup)
    auto *webGroup = new QTreeWidgetItem(filtersTree, {QStringLiteral("Pagini web neproductive")});
    Q_UNUSED(webGroup)
    auto *inactivityGroup = new QTreeWidgetItem(filtersTree, {QStringLiteral("Inactivitate prelungita")});
    Q_UNUSED(inactivityGroup)
    filtersLayout->addWidget(filtersTree, 1);
    auto *filtersNote = new QLabel(
        QStringLiteral("Trage un filtru pe o celula pentru a-l aplica.\n"
                       "(Motorul de reguli/violari nu exista inca in grabber --\n"
                       "panoul e pregatit, dar inert pana atunci.)"),
        m_filtersPanel);
    filtersNote->setWordWrap(true);
    filtersNote->setStyleSheet(QStringLiteral("color: #7a7e86; font-size: 8pt;"));
    filtersLayout->addWidget(filtersNote);
    auto *trashLabel = new QLabel(QStringLiteral("🗑  Scoate filtrul de pe celula"), m_filtersPanel);
    trashLabel->setAlignment(Qt::AlignCenter);
    trashLabel->setStyleSheet(
        QStringLiteral("color: #9fa5ae; border: 1px dashed #55575f; padding: 6px;"));
    filtersLayout->addWidget(trashLabel);
    contentRow->addWidget(m_filtersPanel);

    contentLayout->addLayout(contentRow, 1);

    m_historyView = new HistoryView(m_connection, root);
    m_deviceDetailView = new DeviceDetailView(m_connection, root);
    connect(m_deviceDetailView, &DeviceDetailView::backRequested, this, &MainWindow::closeDeviceDetail);
    connect(m_deviceDetailView, &DeviceDetailView::goToHistoryRequested, this,
            [this](quint32 sessionKey) {
                showHistoryPage();
                m_historyView->openForDevice(sessionKey);
            });
    connect(m_deviceDetailView, &DeviceDetailView::renameRequested, this,
            [this](quint32 sessionKey, const QString &currentName) {
                bool accepted = false;
                const QString name = QInputDialog::getText(
                    this, QStringLiteral("Redenumeste angajatul"),
                    QStringLiteral("Nume (doar local, in acest Viewer):"), QLineEdit::Normal,
                    currentName, &accepted).trimmed();
                if (!accepted || name.isEmpty() || name == currentName) {
                    return;
                }
                m_deviceNameOverrides.insert(sessionKey, name);
                saveSettings();
                for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
                    if (tile->sessionKey() == sessionKey) {
                        tile->setDisplayName(deviceDisplayName(sessionKey));
                    }
                }
                if (m_openDeviceKey == sessionKey) {
                    m_deviceDetailView->setDisplayName(deviceDisplayName(sessionKey));
                }
                refreshHistoryDevices();
            });

    connect(&m_connection, &ViewerConnection::monitorClosed, this, &MainWindow::removeWindowStream);

    m_contentStack = new QStackedWidget(root);
    m_contentStack->addWidget(m_trackerPage);
    m_contentStack->addWidget(m_historyView);
    m_contentStack->addWidget(m_deviceDetailView);
    rootLayout->addWidget(m_contentStack, 1);
    setCentralWidget(root);
    statusBar()->setSizeGripEnabled(false);

    relayoutCurrentTab();

    m_settingsDialog = new QDialog(this);
    m_settingsDialog->setWindowTitle(QStringLiteral("Connection settings"));
    m_settingsDialog->setModal(false);
    m_settingsDialog->setMinimumWidth(480);
    auto *settingsLayout = new QVBoxLayout(m_settingsDialog);
    auto *form = new QFormLayout;
    form->setSpacing(10);
    m_hostEdit = new QLineEdit(QStringLiteral("127.0.0.1"), m_settingsDialog);
    m_portSpin = new QSpinBox(m_settingsDialog);
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(45870);
    m_tokenEdit = new QLineEdit(m_settingsDialog);
    m_tokenEdit->setEchoMode(QLineEdit::Password);
    m_tokenEdit->setPlaceholderText(QStringLiteral("Token partajat"));
    m_tlsCheck = new QCheckBox(QStringLiteral("Foloseste TLS"), m_settingsDialog);
    m_tlsCheck->setChecked(true);
    m_fingerprintEdit = new QLineEdit(m_settingsDialog);
    m_fingerprintEdit->setPlaceholderText(QStringLiteral("SHA-256 optional pentru certificat local"));
    form->addRow(QStringLiteral("Adresa"), m_hostEdit);
    form->addRow(QStringLiteral("Port"), m_portSpin);
    form->addRow(QStringLiteral("Token"), m_tokenEdit);
    form->addRow(QString(), m_tlsCheck);
    form->addRow(QStringLiteral("Amprenta"), m_fingerprintEdit);
    settingsLayout->addLayout(form);
    auto *notice = new QLabel(QStringLiteral(
        "Viewer-ul accepta numai video si metadate. Conexiunea se reface automat la pornire."),
        m_settingsDialog);
    notice->setWordWrap(true);
    notice->setObjectName(QStringLiteral("privacy"));
    settingsLayout->addWidget(notice);
    // The header itself has no status text or connect/disconnect button
    // (matches TopPanel.qml -- StaterNodes' dot+tooltip is the only
    // in-header indicator); this dialog is where the detailed live status
    // and the actual connect/disconnect action live instead.
    m_statusLabel = new QLabel(QStringLiteral("●  Deconectat"), m_settingsDialog);
    m_statusLabel->setObjectName(QStringLiteral("status"));
    settingsLayout->addWidget(m_statusLabel);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, m_settingsDialog);
    m_connectButton = buttons->addButton(QStringLiteral("Connect"), QDialogButtonBox::AcceptRole);
    connect(m_connectButton, &QPushButton::clicked, this, &MainWindow::connectOrDisconnect);
    connect(buttons, &QDialogButtonBox::rejected, m_settingsDialog, &QDialog::hide);
    settingsLayout->addWidget(buttons);
}

void MainWindow::applyStyle()
{
    // Font scale (Preferences dialog) multiplies just this base size --
    // the various explicit pt sizes below (brand/nav/menu icons) stay fixed
    // for visual consistency, a scoped simplification of Settings.qml's
    // Fonts.scale (which scales a whole font-metrics system, not a single
    // CSS rule).
    const int baseFontPt = qMax(6, qRound(9 * m_fontScale));
    setStyleSheet(QStringLiteral(R"(
        QMainWindow, QWidget { background: #30323a; color: #e7eaed; font-family: "Segoe UI"; font-size: %1pt; }
        QFrame#header { background: #299f6c; border: none; }
        QFrame#subbar { background: #292c33; border-bottom: 1px solid #17191e; }
        QPushButton#trackerPanelButton { color: #a2a2a4; background: transparent; border: none; padding: 0;
                                         font-family: Roboto; font-size: 12px; font-weight: 700; text-align: left; }
        QPushButton#trackerPanelButton:hover, QPushButton#trackerPanelButton:checked { color: white; }
        QPushButton#trackerPanelButton:pressed { color: #717276; }
        QPushButton#trackerPanelButton:disabled { color: #6f7176; }
        QLabel#brand { background: #199466; color: white; padding-left: 18px; font-size: 14pt; font-weight: 700; }
        QToolButton#menuButton { background: #188c62; color: white; border: none; font-size: 20pt; }
        QToolButton#menuButton:hover { background: #147a55; }
        QToolButton#headerIcon { background: transparent; border: none; padding: 0; }
        QToolButton#headerIcon:hover { background: rgba(0,0,0,0.10); }
        QLabel#status { color: #d9fff1; padding: 0 10px; }
        QLabel#status[connected="true"] { color: white; }
        QPushButton#flatButton { color: #b8bdc5; background: transparent; border: none; padding: 6px; }
        QPushButton#flatButton:hover { color: white; }
        QPushButton#historyPill, QComboBox#historyPill { color: #d7dadd; background: #3a3d45; border: none; border-radius: 4px; padding: 2px 8px; font-size: 8pt; text-align: left; }
        QPushButton#historyPill:hover { background: #454850; }
        QComboBox#historyPill::drop-down { border: none; }
        QPushButton#historyIconPill { color: #d7dadd; background: #3a3d45; border: none; border-radius: 11px; font-size: 8pt; }
        QPushButton#historyIconPill:hover { background: #454850; }
        QPushButton#historyIconPill:disabled { color: #6f747d; }
        QToolButton#plus { color: #d7dadd; background: transparent; border: none; font-size: 15pt; padding: 0 5px; }
        QToolButton#plus:hover { color: white; }
        QTabBar#tabs::tab { background: #383b43; color: #9fa5ae; padding: 8px 36px; border: 1px solid #292b32; }
        QTabBar#tabs::tab:selected { color: white; border-bottom: 2px solid #1da06f; }
        QWidget#monitorArea { background: #3e4048; }
        QLabel#privacy { color: #aab0b8; padding: 10px; background: #343740; }
        QWidget#detailHeader { background: #383b43; border-bottom: 1px solid #17191e; }
        QWidget#statsPanel { background: #34363e; border-left: 1px solid #17191e; }
        QFrame#filtersPanel { background: #34363e; border-left: 1px solid #17191e; }
        QPushButton#primaryButton { background: #1da06f; color: white; border: none; padding: 8px 12px; font-weight: 600; }
        QPushButton#primaryButton:hover { background: #21b780; }
        QLineEdit, QSpinBox { background: #262930; border: 1px solid #4d515b; padding: 7px; selection-background-color: #1da06f; }
        QLineEdit:focus, QSpinBox:focus { border-color: #21b780; }
        QPushButton { background: #3c4048; border: 1px solid #50545e; padding: 7px 12px; }
        QPushButton:hover { background: #484d57; }
        QPushButton:disabled { color: #6f747d; }
        QCheckBox { spacing: 8px; }
        QScrollArea { background: transparent; }
        QToolTip { background: #23252b; color: #e7eaed; border: 1px solid #4d515b; padding: 3px 6px; }
        QMenu { background: #353840; color: white; border: 1px solid #202228; padding: 5px; }
        QMenu::item { padding: 10px 28px; }
        QMenu::item:selected { background: #1da06f; }
        QDialog { background: #30323a; }
        QStatusBar { background: #272a30; color: #f2a4a4; min-height: 18px; }
        QLabel#historyPreview { background: #05080f; border: 1px solid #4b4e57; }
        QLabel#historyTime { font-weight: 700; padding: 0 8px; }
    )").arg(baseFontPt));
}

void MainWindow::clearMonitors()
{
    const auto monitors = m_monitors;
    m_monitors.clear();
    m_monitorNames.clear();
    m_deviceUsernames.clear();
    m_devicePrimaryStream.clear();
    m_deviceMonitorStreams.clear();
    m_deviceWindowStreams.clear();
    m_streamDeviceKey.clear();
    m_windowStreamIds.clear();
    qDeleteAll(m_deviceTiles);
    m_deviceTiles.clear();
    for (MonitorWidget *monitor : monitors) {
        monitor->deleteLater();
    }
    for (TrackerTab &tab : m_tabs) {
        tab.cells.clear();
    }
    m_selectedStream = 0;
    m_agentLabel->hide();
    if (m_contentStack->currentWidget() == m_deviceDetailView) {
        closeDeviceDetail();
    }
    relayoutCurrentTab();
    refreshHistoryDevices();
}

void MainWindow::refreshHistoryDevices()
{
    QHash<quint32, QString> deviceNames;
    for (auto it = m_devicePrimaryStream.cbegin(); it != m_devicePrimaryStream.cend(); ++it) {
        deviceNames.insert(it.key(), deviceDisplayName(it.key()));
    }
    m_historyView->setDevices(deviceNames, m_deviceMonitorStreams, m_monitorNames);
}

namespace {
// TrackerQuadratorGrid.qml / Controls/ScrollView.qml constants: 3px between
// cells, a 5px scroll handle plus its 4+3 margins = 12px for either bar, and
// 60px-thick fictive half-cells.
constexpr int kGridSpacing = 3;
constexpr int kGridScrollBarSize = 12;
constexpr int kFictiveCellSize = 60;
constexpr int kGridBestColumns = 2;
constexpr int kGridBestRows = 2;

struct CellSpan { int begin; int end; };

// QuadratorGrid.qml's makeCellSize(), line for line: `amount` cells share
// `size` (with `spacing` before, between and after them); the first
// bigAmount get ceil(cellSize), the rest floor(cellSize); entries past
// `amount` (up to amountWithExtra) continue the same pattern, which is what
// gives overflow rows the same height as the visible ones.
QList<CellSpan> makeCellSize(int size, int amount, int amountWithExtra, int spacing)
{
    QList<CellSpan> res;
    if (amount) {
        const double cellSize = static_cast<double>(size - (amount + 1) * spacing) / amount;
        const int bigCellSize = static_cast<int>(std::ceil(cellSize));
        const int smallCellSize = static_cast<int>(std::floor(cellSize));
        const int bigAmount = static_cast<int>(std::floor(amount - (bigCellSize - cellSize) * amount + 0.5));
        int accumuler = spacing;
        for (int idx = 0; idx < amountWithExtra; ++idx) {
            const int cell = idx < bigAmount ? bigCellSize : smallCellSize;
            res.append({accumuler, accumuler + cell});
            accumuler += cell + spacing;
        }
    }
    return res;
}

int ceilDiv(int value, int divisor)
{
    return divisor > 0 ? (value + divisor - 1) / divisor : 0;
}

// TrackerQuadrator.qml's trimTail(): drops trailing empty cells.
template <typename Cells>
void trimTail(Cells &cells)
{
    while (!cells.isEmpty() && cells.constLast().tileId == 0) {
        cells.removeLast();
    }
}

// TrackerQuadrator.qml's collapseEmptyRowsColumns(): removes fully empty
// rows, then fully empty columns, never going below gridBestRows/Columns.
// Returns {columns, rows}.
template <typename Cells>
QPair<int, int> collapseEmptyRowsColumns(Cells &cells, int columns, int rows)
{
    const auto occupied = [&cells](int index) {
        return index >= 0 && index < cells.size() && cells.at(index).tileId != 0;
    };
    for (int row = 0; row < rows && rows > kGridBestRows;) {
        bool empty = true;
        for (int column = 0; column < columns; ++column) {
            if (occupied(row * columns + column)) {
                empty = false;
                break;
            }
        }
        if (empty) {
            const int start = qMin(row * columns, static_cast<int>(cells.size()));
            cells.remove(start, qMin(columns, static_cast<int>(cells.size()) - start));
            --rows;
        } else {
            ++row;
        }
    }
    for (int column = 0; column < columns && columns > kGridBestColumns;) {
        bool empty = true;
        for (int row = 0; row < rows; ++row) {
            if (occupied(row * columns + column)) {
                empty = false;
                break;
            }
        }
        if (empty) {
            for (int row = rows - 1; row >= 0; --row) {
                const int index = row * columns + column;
                if (index < cells.size()) {
                    cells.removeAt(index);
                }
            }
            --columns;
        } else {
            ++column;
        }
    }
    return {qMax(columns, kGridBestColumns), qMax(rows, kGridBestRows)};
}
} // namespace

void MainWindow::relayoutCurrentTab()
{
    // Full rebuild each time, like QuadratorGrid.qml's mkCells4View (re-run
    // on every cells/size/layout change).
    for (QWidget *widget : std::as_const(m_emptyCellWidgets)) {
        widget->hide();
        widget->deleteLater();
    }
    m_emptyCellWidgets.clear();
    for (DeviceTileWidget *tile : std::as_const(m_deviceTiles)) {
        tile->hide();
    }

    if (m_tabs.isEmpty() || !m_gridScroll) {
        return;
    }
    const TrackerTab &tab = m_tabs[m_currentTabIndex];
    if (m_gridStack) {
        m_gridStack->setCurrentWidget(m_gridScrollPage);
    }

    const QString &layout = tab.gridLayout;
    const bool isSimple = layout == QStringLiteral("simple");
    const bool vertical = tab.gridLayoutVertical;
    const int rows4View = isSimple ? tab.gridRows : (vertical ? 5 : 4);
    const int columns4View = isSimple ? tab.gridColumns : (vertical ? 2 : 4);
    if (rows4View <= 0 || columns4View <= 0) {
        return;
    }
    // QuadratorGrid.qml binds hscrollVisible = ("simple" === layout) and
    // vscrollVisible = true; the scroll bars' room is always reserved.
    const bool hscrollVisible = isSimple;
    const int targetWidth = m_gridScroll->width();
    const int targetHeight = m_gridScroll->height();

    struct ViewCell {
        quint32 tileId = 0; // 0 = empty cell
        QList<int> indices;
        GridFillTarget target;
        QRect rect;
    };
    QList<ViewCell> cells;

    int mincellsForLayout = 1;
    if (isSimple) {
        mincellsForLayout = 0;
    } else if (layout == QStringLiteral("f22p02f22") || layout == QStringLiteral("f22p03f22")
               || layout == QStringLiteral("f22p20f22")) {
        mincellsForLayout = 2;
    }
    for (int idx = 0; idx < qMax(static_cast<int>(tab.cells.size()), mincellsForLayout); ++idx) {
        ViewCell cell;
        cell.tileId = idx < tab.cells.size() ? tab.cells.at(idx).tileId : 0;
        cell.target.indexInGrid = static_cast<int>(cells.size());
        cells.append(cell);
    }

    // The named templates: which grid indices (row-major over columns4View)
    // the first one or two cells span. Every other cell takes the next
    // unused index, so cell 0 is always the (first) big cell.
    const int l0 = 0;
    const int l1 = l0 + columns4View;
    const int l2 = l1 + columns4View;
    const int l3 = l2 + columns4View;
    const auto block = [columns4View](int firstRowBegin, int col, int colSpan, int rowSpan) {
        QList<int> indices;
        for (int r = 0; r < rowSpan; ++r) {
            for (int c = 0; c < colSpan; ++c) {
                indices.append(firstRowBegin + r * columns4View + col + c);
            }
        }
        return indices;
    };
    const int n = static_cast<int>(cells.size());
    if (layout == QStringLiteral("f44")) {
        if (columns4View >= 4 && n >= 1) cells[0].indices = block(l0, 0, 4, 4);
    } else if (layout == QStringLiteral("f33")) {
        if (columns4View >= 3 && n >= 1) cells[0].indices = block(l0, 0, 3, 3);
    } else if (layout == QStringLiteral("p10f22")) {
        if (columns4View >= 2 && n >= 1) cells[0].indices = block(l0, (columns4View - 2) / 2, 2, 2);
    } else if (layout == QStringLiteral("p02f22")) {
        if (columns4View >= 2 && n >= 2) cells[0].indices = block(l2, 0, 2, 2);
    } else if (layout == QStringLiteral("p01f22")) {
        if (columns4View >= 2 && n >= 2) cells[0].indices = block(l1, 0, 2, 2);
    } else if (layout == QStringLiteral("p03f22")) {
        if (columns4View >= 2 && n >= 2) cells[0].indices = block(l3, 0, 2, 2);
    } else if (layout == QStringLiteral("f22")) {
        if (columns4View >= 2 && n >= 1) cells[0].indices = block(l0, 0, 2, 2);
    } else if (layout == QStringLiteral("f22p02f22")) {
        if (columns4View >= 2 && n >= 2) {
            cells[0].indices = block(l0, 0, 2, 2);
            cells[1].indices = block(l2, 0, 2, 2);
        }
    } else if (layout == QStringLiteral("f22p03f22")) {
        if (columns4View >= 2 && n >= 2) {
            cells[0].indices = block(l0, 0, 2, 2);
            cells[1].indices = block(l3, 0, 2, 2);
        }
    } else if (layout == QStringLiteral("f22p20f22")) {
        if (columns4View >= 4 && n >= 2) {
            cells[0].indices = block(l0, 0, 2, 2);
            cells[1].indices = block(l0, 2, 2, 2);
        }
    } else if (layout == QStringLiteral("f34")) {
        if (columns4View >= 3 && n >= 1) cells[0].indices = block(l0, 0, 3, 4);
    }

    // calculatedRows is only how far makeCellSize continues the row
    // pattern; extra headroom on top of the original's +5 changes nothing
    // visible and keeps spanning cells from ever indexing past it.
    const int calculatedRows = qMax(rows4View, ceilDiv(n, columns4View)) + 5 + 32;
    const QList<CellSpan> cellPosX =
        makeCellSize(targetWidth - kGridScrollBarSize, columns4View, columns4View, kGridSpacing);
    const QList<CellSpan> cellPosY = makeCellSize(
        targetHeight - (hscrollVisible ? kGridScrollBarSize : 0), rows4View, calculatedRows, kGridSpacing);
    const auto spanX = [&cellPosX](int column) {
        return cellPosX.at(qBound(0, column, static_cast<int>(cellPosX.size()) - 1));
    };
    const auto spanY = [&cellPosY](int row) {
        return cellPosY.at(qBound(0, row, static_cast<int>(cellPosY.size()) - 1));
    };
    QSet<int> usedIndices;
    int maxIndex = 0;

    for (ViewCell &cell : cells) {
        const QList<int> indices = cell.indices.isEmpty() ? QList<int>{0} : cell.indices;
        int xBegin = INT_MAX, xEnd = INT_MIN, yBegin = INT_MAX, yEnd = INT_MIN;
        for (int index : indices) {
            while (usedIndices.contains(index)) {
                ++index;
            }
            usedIndices.insert(index);
            maxIndex = qMax(maxIndex, index);
            const int column = index % columns4View;
            const int row = index / columns4View;
            xBegin = qMin(xBegin, spanX(column).begin);
            xEnd = qMax(xEnd, spanX(column).end);
            yBegin = qMin(yBegin, spanY(row).begin);
            yEnd = qMax(yEnd, spanY(row).end);
        }
        cell.rect = QRect(xBegin, yBegin, xEnd - xBegin, yEnd - yBegin);
    }

    // Fill the rest of the base grid (and pad the last row) with empty cells.
    for (int index = 0; index < rows4View * columns4View || index <= maxIndex || index % columns4View != 0;
         ++index) {
        if (usedIndices.contains(index)) {
            continue;
        }
        usedIndices.insert(index);
        maxIndex = qMax(maxIndex, index);
        const CellSpan x = spanX(index % columns4View);
        const CellSpan y = spanY(index / columns4View);
        ViewCell cell;
        cell.target.indexInGrid = static_cast<int>(cells.size());
        cell.rect = QRect(x.begin, y.begin, x.end - x.begin, y.end - y.begin);
        cells.append(cell);
    }

    const int visibleRows = ceilDiv(maxIndex + 1, columns4View);
    const int fictiveBegin = static_cast<int>(cells.size());

    // The fictive half-cells: a 60px row below the grid (every layout) and,
    // for "simple", a 60px column to its right -- outside the viewport-sized
    // grid, reached by scrolling.
    for (int column = 0; column < columns4View; ++column) {
        ViewCell cell;
        cell.target = {static_cast<int>(cells.size()), visibleRows, column};
        const CellSpan x = spanX(column);
        cell.rect = QRect(x.begin, spanY(visibleRows - 1).end + kGridSpacing, x.end - x.begin, kFictiveCellSize);
        cells.append(cell);
    }
    if (isSimple) {
        for (int row = 0; row < visibleRows; ++row) {
            ViewCell cell;
            cell.target = {-1, row, columns4View};
            const CellSpan y = spanY(row);
            cell.rect = QRect(spanX(columns4View - 1).end + kGridSpacing, y.begin, kFictiveCellSize, y.end - y.begin);
            cells.append(cell);
        }
    }

    // TrackerQuadrator.qml's cellsLimitReached: the "+" of every empty cell
    // (fictive ones included) goes inert at the cap.
    const bool limitReached = tab.tileCount() >= kMaxTilesPerTab;
    QRect childrenRect;
    for (int i = 0; i < cells.size(); ++i) {
        const ViewCell &cell = cells.at(i);
        const QRect rect(cell.rect.topLeft(), QSize(qMax(0, cell.rect.width()), qMax(0, cell.rect.height())));
        childrenRect = childrenRect.isNull() ? rect : childrenRect.united(rect);
        if (cell.tileId == 0) {
            auto *addTile = new AddDeviceTileWidget(m_monitorContainer);
            addTile->setFictive(i >= fictiveBegin);
            addTile->setLimitReached(limitReached);
            connect(addTile, &AddDeviceTileWidget::addRequested, this,
                    [this, target = cell.target] { openAddDeviceDialog(target); });
            addTile->setGeometry(rect);
            addTile->show();
            m_emptyCellWidgets.append(addTile);
            continue;
        }

        const TrackerTab::TileEntry &entry = tab.cells.at(cell.target.indexInGrid);
        DeviceTileWidget *tile = m_deviceTiles.value(entry.tileId, nullptr);
        if (!tile) {
            tile = new DeviceTileWidget(entry.deviceKey, deviceDisplayName(entry.deviceKey),
                                        m_monitorContainer);
            connect(tile, &DeviceTileWidget::opened, this, &MainWindow::openDeviceDetail);
            connect(tile, &DeviceTileWidget::closeRequested, this,
                    [this, tileId = entry.tileId] { removeTile(tileId); });
            connect(tile, &DeviceTileWidget::historyRequested, this, [this](quint32 sessionKey) {
                showHistoryPage();
                m_historyView->openForDevice(sessionKey);
            });
            m_deviceTiles.insert(entry.tileId, tile);
            tile->setAvailableStreams(m_deviceMonitorStreams.value(entry.deviceKey),
                                      m_deviceActiveWindowStream.value(entry.deviceKey, 0),
                                      m_deviceActiveMonitorStream.value(entry.deviceKey, 0));
            for (quint32 monitorStream : m_deviceMonitorStreams.value(entry.deviceKey)) {
                if (MonitorWidget *monitor = m_monitors.value(monitorStream, nullptr)) {
                    tile->updateThumbnail(monitorStream, monitor->currentFrame());
                }
            }
            const QString knownState = m_deviceSessionState.value(entry.deviceKey);
            if (!knownState.isEmpty()) {
                tile->setSessionState(knownState);
            }
            const QPair<QString, QString> activity = m_deviceActivity.value(entry.deviceKey);
            tile->setActivity(activity.first, activity.second, deviceCategory(entry.deviceKey, activity.first));
        }
        tile->setParent(m_monitorContainer);
        tile->setGeometry(rect);
        tile->show();
    }

    // TrackerQuadratorGrid.qml's content item: sized to its children (+ the
    // trailing spacing) along a scrolling axis, else to the ScrollView minus
    // the reserved scroll bar and 1px.
    const int contentWidth = hscrollVisible ? childrenRect.right() + 1 + kGridSpacing
                                            : targetWidth - kGridScrollBarSize - 1;
    const int contentHeight = childrenRect.bottom() + 1 + kGridSpacing;
    m_monitorContainer->resize(qMax(0, contentWidth), qMax(0, contentHeight));
}

void MainWindow::fillTarget(const GridFillTarget &target, quint32 deviceKey)
{
    // QuadratorGrid.qml's onWantFill: in "simple" a fictive cell turns into
    // the index it will have once the grid has grown to include it; any
    // other layout just fills the cell's own slot.
    if (m_tabs.isEmpty()) {
        return;
    }
    const TrackerTab &tab = m_tabs.at(m_currentTabIndex);
    if (tab.gridLayout != QStringLiteral("simple")) {
        fillCell(target.indexInGrid, deviceKey, false, 0, 0);
        return;
    }
    if (target.fictiveRow >= 0 || target.fictiveColumn >= 0) {
        const int columns = qMax(target.fictiveColumn + 1, tab.gridColumns);
        const int rows = qMax(target.fictiveRow + 1, tab.gridRows);
        fillCell(target.fictiveRow * columns + target.fictiveColumn, deviceKey, true, columns, rows);
        return;
    }
    fillCell(target.indexInGrid, deviceKey, true, 0, 0);
}

void MainWindow::fillCell(int cell, quint32 deviceKey, bool canCollapse, int minColumns, int minRows)
{
    // TrackerQuadrator.qml's grid onWantFill: grow the grid to minColumns
    // (inserting an empty cell at the end of every row, so existing cells
    // keep their row/column) and minRows first, then fill that exact cell.
    if (m_tabs.isEmpty() || cell < 0) {
        return;
    }
    TrackerTab &tab = m_tabs[m_currentTabIndex];
    QList<TrackerTab::TileEntry> &cells = tab.cells;
    int columns = qMax(tab.gridColumns, kGridBestColumns);
    int rows = qMax(ceilDiv(static_cast<int>(cells.size()), columns), kGridBestRows);
    while (minColumns > columns) {
        for (int idx = columns; idx < cells.size(); idx += columns + 1) {
            cells.insert(idx, TrackerTab::TileEntry{0, 0});
        }
        ++columns;
    }
    while (minRows > rows) {
        ++rows;
    }
    while (cells.size() <= cell) {
        cells.append(TrackerTab::TileEntry{0, 0});
    }
    cells[cell] = TrackerTab::TileEntry{m_nextTileId++, deviceKey};
    if (canCollapse) {
        const QPair<int, int> shape = collapseEmptyRowsColumns(cells, columns, rows);
        columns = shape.first;
        rows = shape.second;
    }
    tab.gridColumns = columns;
    tab.gridRows = rows;
    relayoutCurrentTab();
}

void MainWindow::removeTile(quint32 tileId)
{
    // TrackerQuadrator.qml's grid onWantFree: the cell becomes an empty hole
    // in place (only trailing holes are trimmed); "simple" then collapses
    // any fully empty row/column.
    if (m_tabs.isEmpty()) {
        return;
    }
    TrackerTab &tab = m_tabs[m_currentTabIndex];
    QList<TrackerTab::TileEntry> &cells = tab.cells;
    for (TrackerTab::TileEntry &entry : cells) {
        if (entry.tileId == tileId) {
            entry = TrackerTab::TileEntry{0, 0};
        }
    }
    if (DeviceTileWidget *tile = m_deviceTiles.take(tileId)) {
        tile->deleteLater();
    }
    trimTail(cells);
    int columns = qMax(tab.gridColumns, kGridBestColumns);
    int rows = qMax(ceilDiv(static_cast<int>(cells.size()), columns), kGridBestRows);
    if (tab.gridLayout == QStringLiteral("simple")) {
        const QPair<int, int> shape = collapseEmptyRowsColumns(cells, columns, rows);
        columns = shape.first;
        rows = shape.second;
    }
    tab.gridColumns = columns;
    tab.gridRows = rows;
    relayoutCurrentTab();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}

void MainWindow::loadSettings()
{
    QSettings settings;
    m_hostEdit->setText(settings.value(QStringLiteral("connection/host"),
                                       QStringLiteral("127.0.0.1")).toString());
    m_portSpin->setValue(settings.value(QStringLiteral("connection/port"), 45870).toInt());
    m_tlsCheck->setChecked(settings.value(QStringLiteral("connection/tls"), true).toBool());
    m_fingerprintEdit->setText(settings.value(QStringLiteral("connection/fingerprint")).toString());
    // Saved (and auto-connected on launch, see the constructor) -- the user
    // explicitly asked not to have to retype it every time on their own,
    // self-hosted setup. Was deliberately left unsaved before, for a
    // multi-user-machine privacy concern that doesn't apply here.
    m_tokenEdit->setText(settings.value(QStringLiteral("connection/token")).toString());

    m_language = settings.value(QStringLiteral("preferences/language"), QStringLiteral("ro")).toString();
    QLocale::setDefault(
        QLocale(m_language == QStringLiteral("en") ? QLocale::English : QLocale::Romanian));
    m_fontScale = settings.value(QStringLiteral("preferences/fontScale"), 1.0).toDouble();
    m_tooltipsEnabled = settings.value(QStringLiteral("preferences/tooltipsEnabled"), true).toBool();
    m_simpleMode = settings.value(QStringLiteral("preferences/simpleMode"), false).toBool();

    // Local rename overrides -- keyed by deviceKey, which is only stable
    // for as long as KikiHost keeps reporting the same sessionId for
    // that machine (see the member comment). Best-effort until there's a
    // real, persistent employee identity to key on instead.
    m_deviceNameOverrides.clear();
    const int overrideCount = settings.beginReadArray(QStringLiteral("deviceNameOverrides"));
    for (int i = 0; i < overrideCount; ++i) {
        settings.setArrayIndex(i);
        const quint32 key = settings.value(QStringLiteral("deviceKey")).toUInt();
        const QString name = settings.value(QStringLiteral("name")).toString();
        if (key != 0 && !name.isEmpty()) {
            m_deviceNameOverrides.insert(key, name);
        }
    }
    settings.endArray();
}

void MainWindow::saveSettings() const
{
    QSettings settings;
    settings.setValue(QStringLiteral("connection/host"), m_hostEdit->text());
    settings.setValue(QStringLiteral("connection/port"), m_portSpin->value());
    settings.setValue(QStringLiteral("connection/tls"), m_tlsCheck->isChecked());
    settings.setValue(QStringLiteral("connection/fingerprint"), m_fingerprintEdit->text());
    settings.setValue(QStringLiteral("connection/token"), m_tokenEdit->text());

    settings.setValue(QStringLiteral("preferences/language"), m_language);
    settings.setValue(QStringLiteral("preferences/fontScale"), m_fontScale);
    settings.setValue(QStringLiteral("preferences/tooltipsEnabled"), m_tooltipsEnabled);
    settings.setValue(QStringLiteral("preferences/simpleMode"), m_simpleMode);

    settings.beginWriteArray(QStringLiteral("deviceNameOverrides"));
    int index = 0;
    for (auto it = m_deviceNameOverrides.cbegin(); it != m_deviceNameOverrides.cend(); ++it) {
        settings.setArrayIndex(index++);
        settings.setValue(QStringLiteral("deviceKey"), it.key());
        settings.setValue(QStringLiteral("name"), it.value());
    }
    settings.endArray();
}

// GridsPanel declares Q_OBJECT in this .cpp, so AUTOMOC needs this.
#include "mainwindow.moc"
