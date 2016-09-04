# Scramble
An attempt at a real-time rendering engine in C++ using the OpenGL API.
Hopefully a very ironic title.

## What it does
It sets up for you a navigable environment with a simple room scene
(brilliantly modeled by me) equipped with toggleable light sources.
**THE ROOM WILL BE COMPLETELY DARK AT FIRST**, don't fret. Use `c`,`r`,
`g` and `f` keys to toggle light sources, `w`,`a`,`s` and `d` to
navigate and the mouse to look around. There's a few assets synced in
the repo, and you can load any other by editing the `scene.cc` unit.
Or, you know, do whatever you like! Rework the code, load your own
assets, etc... My only hope is that this project makes your life
easier somehow. There's licensing info all around but all you
really gotta do is minimally state your changes. Following are
sections containing features, dependencies and build instructions.
a

## Features so far
- [x] Smooth, free navigation (euler angles, clamped)
- [x] Basic shader load & link modular framework
- [x] Fully de-coupled mesh loading
- [x] Modular texture mapping
- [x] Normal mapping
- [x] Optimized reflections
- [x] Simple 3D environment scene
- [ ] Complex 3D environment scene
- [ ] Custom framebuffers (allows for effects like off-screen AA, color blending, etc)

## General design
![General Design](http://i.imgur.com/7wDU86u.png "General Design")
- Green modules are functional and belong to the framework
- Blue modules are functional and belong to the user
- Yellow modules are yet under heavy redesigning

## Dependencies
* CMake >= 2.8
* clang compiler suite
* OpenGL >= 3.3 definition
* GLEW (OpenGL extension wrangler)
* GLFW (window spawning, event handling)
* GLM (vector objects, matrix transforms)
* Assimp (asset import lib)
* SOIL for texture loading

Following are the package version reads for my system:
###### OpenGL/Mesa
```
[danilo@revenant ~]$ glxinfo | grep "OpenGL core profile"
OpenGL core profile version string: 3.3 (Core Profile) Mesa 12.0.1
OpenGL core profile shading language version string: 3.30
OpenGL core profile context flags: (none)
OpenGL core profile profile mask: core profile
OpenGL core profile extensions:
```
###### GLEW
```
$ glewinfo | grep version
GLEW version 2.0.0
OpenGL version 3.0 Mesa 12.0.1 is supported
```
###### Assimp
```
$ assimp version
------------------------------------------------------ 
Open Asset Import Library ("Assimp", http://assimp.sourceforge.net) 
 -- Commandline toolchain --
------------------------------------------------------ 
Version 3.2 -shared -st (GIT commit 0)
```
###### clang/LLVM
```
$ clang --version
clang version 3.8.1 (tags/RELEASE_381/final)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```
###### CMake
```
$ cmake --version
cmake version 3.6.1
CMake suite maintained and supported by Kitware (kitware.com/cmake).
```
###### GLFW
```
$ pacman -Qs glfw
local/glfw-x11 3.2.1-1
    A free, open source, portable framework for graphical application
    development (x11)
```
###### GLM
```
$ pacman -Qs glm
local/glm 0.9.7.6-1
    C++ mathematics library for 3D software based on the OpenGL Shading
    Language (GLSL) specification
```
###### SOIL
```
$ pacman -Qs soil
local/soil 1.16-2
    A tiny C lib primarily for loading textures into OpenGL
```
GLEWinfo shows us OpenGL 3.0 only, but Mesa implements core profile 3.3 since version 10 and it is provided if requested at context creation, which we do, so it's fine. GLXinfo shows us core profile capability as far as 3.3 and, even though Mesa defaults to 3.0 because of compatibility context issues, it is there. As you probably guessed, also, Arch's bleeding edge packages are likely more recent than the versions you actually need, and this is certain for CMake and clang. Even so, the more info on build context I can provide, the better.

## Building
Standard procedure for almost every small CMake based project; use a separate
`build` directory so things don't get messed up. Here I'll be using a few useful
flags, but setting build type to `Debug` (because debug info is still very
important). For a performance-optimized version, one should use the `Release`
builtin config (which works fine as well):
```
# From the project's root directory
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=on -DCMAKE_BUILD_TYPE=Debug ..
make
```
Then, just run the binary:
```
# From the build directory
./scramble
```
When using `Debug` warnings will ensue because of implicit floating point conversions and use of GNU extensions. I can silence these in the future (hopefully by fixing them), but there are no implications on functionality so far. The precision loss is mostly only theoretical.

## Why the f#%k do you keep your assets on the repo?
Look, I'm sorry. This project was originally to be presented and I was kinda desperate so things happened. Now I don't wanna remove them because it's actually practical in case one just wants to clone the repo and test a default buid.

## License information

   Copyright 2016 Danilo Nery 

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this code except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
