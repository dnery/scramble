# Scramble
An attempt at a real-time rendering engine in C++ using the OpenGL API.
Hopefully a very ironic title.

## What it does
By default, it sets up for you a navigable environment with a simple room scene
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

## How's it looking?
Here are a couple of shots demo'ing a complex mesh composition loaded in the
engine (RIP Crytek). Shaders without normal mapping here because the Nanosuit
model ([taken from TF3DM.com](http://tf3dm.com/3d-model/crysis-2-nanosuit-2-97837.html))
came with screwed up normal textures, so to avoid an overly specific solution I
simply removed them from the equation. The model is amazing, though, and it
still looks very good with only Blinn-Phong reflections.
[![Scramble_013.png](https://s22.postimg.org/o2l1og0g1/Scramble_013.png)](https://postimg.org/image/6cjd3emv1/)
[![Scramble_012.png](https://s13.postimg.org/8myrrfbd3/Scramble_012.png)](https://postimg.org/image/53cu1m8n7/)

And here is a simple room scene I put up in blender ([textures taken from OpenGameArt.org](http://opengameart.org/content/majestical-metal-sci-fi-station<Paste>))
rendered under different lighting schemes and colors, featuring normal mapping.
It was quick work but the textures are very well made and I think the scene
looks good (it's prettier when you can navigate through it real-time, trust
me). Not showing the flashlight here because it's mainly for debugging the
reflection attenuation and not particularly interesting either.
[![Scramble_009.png](https://s17.postimg.org/mpo5dzej3/Scramble_009.png)](https://postimg.org/image/uiet5yki3/)
[![Scramble_008.png](https://s17.postimg.org/cs3pdx7of/Scramble_008.png)](https://postimg.org/image/kkud5wdnf/)
[![Scramble_010.png](https://s11.postimg.org/jx1qfe9zn/Scramble_010.png)](https://postimg.org/image/brjoh8lqn/)

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
  MIT License
  
  Copyright (c) 2016 Danilo Nery
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
