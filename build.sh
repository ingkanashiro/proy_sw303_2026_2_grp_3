if [ ! -f "lib/raylib.h" ]; then
    echo "'lib/raylib.h' no existe"
    exit 1
fi
if [ ! -f "lib/linux/libraylib.a" ]; then
    echo "'lib/linux/libraylib.a' no existe"
    exit 1
fi

gcc "src/main.c" -I"lib" -L"lib/linux" -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o "bin/main" -O3

# hacer chmod +x build.sh la primera vez
# de ahi en adelante nomas haces ./build.sh para compilar``