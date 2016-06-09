cmake_minimum_required(VERSION 3.3)

project(scramble)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lGL -lGLEW -lglfw -lSOIL -lassimp -Wall")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ftrapv -fsanitize=address")

set(CMAKE_CXX_FLAGS_RELEASE "-02 -march=native")

set(SOURCE_FILES
        src/main.cc
        src/debug.hh
        src/scene_type.hh
        src/object_type.hh
        src/mesh.cc src/mesh.hh
        src/model.cc src/model.hh
        src/scene.cc src/scene.hh
        src/object.cc src/object.hh
        src/camera.cc src/camera.hh
        src/shader.cc src/shader.hh
        src/program.cc src/program.hh
        src/ext/platform.cc src/ext/platform.hh
        src/window_manager.cc src/window_manager.hh
)

add_executable(scramble ${SOURCE_FILES})
