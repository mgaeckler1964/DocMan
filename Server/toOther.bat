set var=%cd%
echo %var%

if "%var%"=="C:\CRESD\object" goto nix

if "%var%" GTR "E" goto netz

set source=C:\CRESD\Source\DocMan\Server
set target=M:\wwwroot\DocMan

goto end

:netz
set source=M:\wwwroot\DocMan
set target=C:\CRESD\Source\DocMan\Server

:end

mirror -l %source% %target%

:nix

pause