if not exist "lib\raylib.h" (
    echo "lib\raylib.h" no existe
    exit /b 1
)

if not exist "lib\win\libraylib.a" (
    echo "lib\win64\libraylib.a" no existe
    exit /b 1
)

if not exist "bin" mkdir "bin"

gcc "src/main.c" -I"lib" -L"lib/win" -lraylib -lgdi32 -lwinmm -static -Wall -Wextra -o "bin/main.exe"

:: correr asi nomas, usualmente ./build.bat