# raylibGames
Alguns exemplos de jogos simples utilizando a bibloteca RayLib e a linguagem Ansi C


---
## Para configuarar os pré requisitos da biblioteca 

### No Linux
Instale as dependencias para as bibliotecas de video e audio
```
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

### No windows 
utilize o compilador MinGW-w64 (64 bits)
```
https://github.com/skeeto/w64devkit/releases/download/v2.0.0/w64devkit-x64-2.0.0.exe
```


## Para compilar utilize:

### No Windows
```
mingw32-make
```
### No Linux
```
make
```

### Para executar utilize:

### No Windows
```
mingw32-make run
```
### No Linux
```
make run
```

## Caso queira compilar manualmente, sem makefile, utilize:
```
## Para compilar seu programa utilize
```
gcc seuJogo.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o seuJogo
```