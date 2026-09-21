# Personal Screen Viewer

Viewer desktop Qt 6 pentru afisarea explicita si autorizata a ecranelor trimise
de un agent propriu. Proiectul nu contine control la distanta, audio, clipboard,
transfer de fisiere, keylogging, instalare ca serviciu sau mecanisme de ascundere.

## Ce este implementat

- conexiune TCP/TLS viewer → agent, cu token de autentificare;
- pinning SHA-256 optional pentru certificate TLS locale;
- fluxuri multiple, cate unul pentru fiecare monitor;
- cadre complete PNG/JPEG si subcadre delta;
- validarea dimensiunilor, secventei si limitelor subcadrelor;
- metadate minimale (aplicatie/stare idle), latenta si numar de secventa;
- mod demonstrativ fara agent;
- salvare manuala a unei capturi PNG.

Tokenul nu este salvat in setari. Viewer-ul refuza sa trimita un token printr-o
conexiune necriptata catre alta adresa decat localhost.

## Compilare

Deschideti `CMakeLists.txt` in Qt Creator si selectati kitul Qt 6.11.2 MinGW 64-bit,
apoi Build. Din terminal:

```powershell
C:\Qt\Tools\CMake_64\bin\cmake.exe -S . -B build -G Ninja `
  -DCMAKE_PREFIX_PATH=C:\Qt\6.11.2\mingw_64 `
  -DCMAKE_MAKE_PROGRAM=C:\Qt\Tools\Ninja\ninja.exe `
  -DCMAKE_CXX_COMPILER=C:\Qt\Tools\mingw1310_64\bin\g++.exe
C:\Qt\Tools\CMake_64\bin\cmake.exe --build build
```

Verificare headless dupa build:

```powershell
$env:QT_QPA_PLATFORM = "offscreen"
.\build\PersonalScreenViewer.exe --smoke-test
```

Directorul `dist` generat cu `windeployqt` este pachetul portabil. El foloseste
`Qt6Core`, `Qt6Gui`, `Qt6Widgets`, `Qt6Network` si `Qt6Svg`, backend-ul TLS
Windows Schannel, pluginul `qwindows` si runtime-ul MinGW. DLL-urile Qt 5.6.2,
ICU 58 si OpenSSL 1.0.2j din viewer-ul analizat nu sunt copiate, deoarece nu
sunt compatibile cu Qt 6 si nu sunt necesare acestei implementari.

## Protocol PSV1

Fiecare mesaj are un header big-endian de 32 bytes:

| Camp | Marime |
|---|---:|
| magic `PSV1` | 4 |
| versiune | 2 |
| tip mesaj | 2 |
| stream/monitor ID | 4 |
| marime payload | 4 |
| secventa | 8 |
| timestamp Unix ms | 8 |

Tipurile sunt `ClientHello`, `AgentHello`, `FullFrame`, `DeltaFrame`, `Metadata`,
`Heartbeat`, `Goodbye` si `AuthResult`. Un `DeltaFrame` incepe cu patru valori
`int32` big-endian (`x`, `y`, `width`, `height`), urmate de imaginea PNG/JPEG a
subcadrului. Daca lipseste cadrul suport, secventa nu este consecutiva ori
dreptunghiul iese din limite, delta este ignorata pana la un nou cadru complet.

Agentul va fi proiectat separat, cu indicator vizibil permanent si pornire
explicita de catre utilizator.
