cmake_minimum_required(VERSION 3.4)
project(scramble)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lGL -lGLEW -lglfw -lSOIL -g -Wall")

set(SOURCE_FILES
        src/main.cc
    src/sdebug.hh
        src/ext/platform.cc src/ext/platform.hh
        src/listener.cc src/listener.hh
        src/program.cc src/program.hh
        src/shader.cc src/shader.hh
        src/object.cc src/object.hh
        src/window.cc src/window.hh
        src/camera.cc src/camera.hh
        src/engine.cc src/engine.hh
)

add_executable(scramble ${SOURCE_FILES})