@echo off
REM Pune binarele proaspat construite in dist\ si reporneste serviciul.
REM
REM KikiHost.exe este tinut blocat de serviciul care ruleaza chiar din dist\,
REM iar KikiViewer.exe de viewerul deschis - deci ambele cer intai oprirea
REM procesului care le foloseste. Oprirea serviciului cere drepturi de
REM administrator.
REM
REM Rulare: click dreapta -> "Run as administrator".

setlocal
set "HERE=%~dp0"
set "VIEWER=%HERE%..\CustomViewer"

net session >nul 2>&1
if errorlevel 1 (
    echo Trebuie rulat ca administrator ^(click dreapta -^> Run as administrator^).
    pause
    exit /b 1
)

echo Inchid viewerul daca ruleaza...
taskkill /im KikiViewer.exe /f >nul 2>&1

echo Opresc serviciul KikiHost...
sc stop KikiHost >nul
for /l %%i in (1,1,20) do (
    sc query KikiHost | find "STOPPED" >nul && goto :stopped
    timeout /t 1 /nobreak >nul
)
echo Serviciul nu s-a oprit la timp.
pause
exit /b 1

:stopped
echo Copiez KikiHost.exe...
copy /y "%HERE%build-current\KikiHost.exe" "%HERE%dist\KikiHost.exe" >nul
if errorlevel 1 (
    echo Copierea KikiHost a esuat.
    pause
    exit /b 1
)

echo Copiez KikiViewer.exe...
copy /y "%VIEWER%\build-current\KikiViewer.exe" "%VIEWER%\dist\KikiViewer.exe" >nul
if errorlevel 1 (
    echo Copierea KikiViewer a esuat ^(mai ruleaza viewerul?^).
    pause
    exit /b 1
)

echo Pornesc serviciul KikiHost...
sc start KikiHost >nul
if errorlevel 1 (
    echo Serviciul nu a pornit; verifica %%LOCALAPPDATA%%\KikiAgent\host.log
    pause
    exit /b 1
)

echo Gata. Ambele binare sunt in dist, serviciul ruleaza.
pause
