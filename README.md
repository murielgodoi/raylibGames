# raylibGames
Alguns exemplos de jogos simples utilizando a bibloteca RayLib e a linguagem Ansi C



## Para recompilar a biblioteca no seu Linux (Ubuntu)

Instale o compilador GCC, make e git (Caso ainda não tenha instalado):
```
sudo apt install build-essential git
```

Instale dependencias para as bibliotecas de video e audio 
```
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

Faça o download (clone) do repositório com o código fonte do Raylib
```
git clone https://github.com/raysan5/raylib.git raylib
cd raylib/src/
```
Compile e installe a biblioteca
```
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED # To make the dynamic shared version.
sudo make install RAYLIB_LIBTYPE=SHARED # Dynamic shared version.
```

Caso encontre problemas do compilador não encontrar a biblioteca atualize com:
```
sudo ldconfig
```
## Para compilar seu programa utilize
```
gcc seuJogo.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o seuJogo
```

## Caso necessite desinstalar a biblioteca utilize:
```
sudo make uninstall RAYLIB_LIBTYPE=SHARED
```
---
## Para compilar e executar 

### No Linux
Instale as dependencias para as bibliotecas de video e audio seguindo as intruções acima

### No windows 
utilize o compilador MinGW-w64 (64 bits)
```
https://github.com/skeeto/w64devkit/releases/download/v2.0.0/w64devkit-x64-2.0.0.exe
```


Para compilar utilize:
```
make
```

Para executar utilize:
```
make run
```
