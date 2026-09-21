# Personal Screen Agent

Agent Qt 6 care captureaza ecranele masinii locale si le transmite catre
Personal Screen Viewer prin protocolul propriu PSV1. Nu contine control la
distanta, audio, clipboard, transfer de fisiere, keylogging, instalare ca
serviciu Windows sau mecanisme de ascundere/pornire automata.

## Ce este implementat

- captura periodica a fiecarui monitor conectat (`QScreen::grabWindow`),
  cu limitare FPS configurabila (1-30);
- detectie de regiune modificata intre doua cadre (comparatie pe o copie
  miniaturizata) pentru a decide intre `FullFrame` si `DeltaFrame`; daca
  regiunea schimbata depaseste un prag din suprafata ecranului, se trimite
  intotdeauna cadrul complet;
- JPEG cu calitate configurabila pentru toate cadrele;
- server TCP/TLS propriu (`AgentServer`), cate o conexiune independenta
  per viewer, fiecare cu propriul cache de "ultimul cadru trimis" si propria
  secventa, astfel incat mai multi viewer pot fi conectati simultan fara sa
  interfereze;
- autentificare obligatorie prin token; conexiunile care declara in
  `ClientHello` capabilitati nepermise (`remoteControl`, `audio`,
  `clipboard`, `fileTransfer`, `keylogging`) sunt refuzate explicit;
- conexiunile necriptate sunt acceptate doar de la `127.0.0.1`/loopback;
  pentru acces de pe alta masina este obligatoriu TLS (certificat + cheie
  PEM incarcate din UI);
- indicator vizibil permanent (bara rosie, mereu deasupra, fara buton de
  inchidere) cat timp agentul transmite, cu numarul de viewer conectati;
- Start/Stop explicit din interfata; nimic nu porneste automat la lansare
  in afara de fereastra agentului;
- metadate de activitate (aplicatia din prim-plan si timpul de idle) pe
  Windows, folosite pentru afisarea din viewer.

Token-ul, portul si setarile TLS sunt salvate local prin `QSettings` pentru
comoditatea repornirii agentului; nimic din acestea nu paraseste masina.

## Compilare

```powershell
$env:Path = "C:\Qt\Tools\mingw1310_64\bin;" + $env:Path

C:\Qt\Tools\CMake_64\bin\cmake.exe -S . -B build-current -G "MinGW Makefiles" `
  -DCMAKE_PREFIX_PATH=C:\Qt\6.11.2\mingw_64 `
  -DCMAKE_CXX_COMPILER=C:\Qt\Tools\mingw1310_64\bin\g++.exe

C:\Qt\Tools\CMake_64\bin\cmake.exe --build build-current -- -j2
```

Pachet portabil:

```powershell
Copy-Item .\build-current\PersonalScreenAgent.exe .\dist\PersonalScreenAgent.exe -Force

C:\Qt\6.11.2\mingw_64\bin\windeployqt.exe --release --compiler-runtime `
  --no-translations --no-system-d3d-compiler --no-opengl-sw `
  --dir dist dist\PersonalScreenAgent.exe
```

## Testare

Smoke test headless (necesita `platforms\qoffscreen.dll` in `dist`, copiat
manual din `C:\Qt\6.11.2\mingw_64\plugins\platforms\qoffscreen.dll` — nu
face parte din pachetul de productie, e doar pentru verificare automata):

```powershell
$env:QT_QPA_PLATFORM = "offscreen"
.\dist\PersonalScreenAgent.exe --smoke-test
```

Pornire fara interactiune (utila pentru teste de interoperabilitate cu
viewer-ul sau scripturi), plaintext limitat la localhost:

```powershell
.\dist\PersonalScreenAgent.exe --autostart-port=8934 --autostart-token=xxxx --autostart-no-tls
```

Fara `--autostart-no-tls`, `--autostart-port`/`--autostart-token` necesita
si certificatul/cheia TLS deja completate in setarile salvate anterior.

## Protocol PSV1

Vezi `README.md` din `CustomViewer` pentru formatul complet al header-ului
si al tipurilor de mesaj; agentul foloseste acelasi `frameprotocol.h/.cpp`
byte-identic. Secventa per conexiune si per monitor este proprie fiecarui
viewer conectat: primul cadru trimis pe un flux este intotdeauna
`FullFrame` cu secventa 1, iar cadrele urmatoare cresc secventa cu 1 la
fiecare trimitere (complet sau delta), asa cum asteapta viewer-ul.

## Urmatorii pasi posibili

- generare automata de certificat auto-semnat (in prezent trebuie furnizat
  manual, ca fisiere PEM);
- reconectare/backoff daca viewer-ul se deconecteaza si se reconecteaza
  rapid (in prezent fiecare conexiune e tratata independent, fara stare
  pastrata dupa deconectare);
- selectie per-monitor (activare/dezactivare captura pentru un ecran
  anume) direct din UI.
