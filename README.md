# Scramble
An attempt at a real-time rendering engine in C++ using the OpenGL API.
Hopefully a very ironic title.

## What it does so far
It sets up for you a navigable environment with a sample nanosuit object (RIP
Crytek) and a couple of light sources to observe specular highlights. the
following sections are also pertinent to the current state of things, meaning
they will change (expand) as the milestones are achieved.

## Feature checklist
- [x] Fully free navigation (euler angles rotation, clamped)
- [x] Basic shader load+link modular framework
- [x] Fully de-coupled mesh loading
- [x] Modular texture mapping
- [ ] Complex 3D environment scene
- [x] Optimized lighting & reflections
- [ ] Custom framebuffers
- [x] Normal mapping

## General Design
![General Design](http://i.imgur.com/7wDU86u.png "General Design")
- Greens are fully functional and belong to the framework
- Blues are fully functional and belong to the user
- Yellows are yet under heavy redesigning

## Dependencies
* clang/LLVM compiler suite, Cmake for building
* OpenGL >= 3.3 definition (prototypes provided by GLEW)
* GLFW (window spawning and event handling)
* GLM (vector objects and matrix transforms)
* SOIL for image loading (textures)

## Building
Standard procedure for almost every small CMake based project; use a separate
`build` directory so things don't get messed up. Here I'll be using a few useful
flags, but setting build type to `Debug` (because debug info is still very
important). For a performance-optimized version, one should use the `Release`
builtin config:
```bash
# From the project's root directory
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=on -DCMAKE_BUILD_TYPE=Debug ..
make
```
Then, just run the binary:
```bash
# From the build directory
./scramble
```
