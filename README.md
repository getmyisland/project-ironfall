# dyxide

Dyxide is to be a faithful remake of the game "IRONFALL Invasion", which was released for the Nintendo 3DS in 2015.

The project is still at a very early stage and it may take a while for it to take shape.

I am in no way affiliated with VD-dev or Nintendo. I'm just a fan of the game who wants to recreate parts of it for the PC.

## Setup (WIP)

```
git clone https://github.com/getmyisland/dyxide.git
cd dyxide
git submodule update --init --recursive
```

Install the following packages in vcpkg:

```
zlib
libpng
freetype
tinyxml2
```

Build with CMake:

```
cmake -B build
cmake --build build
```