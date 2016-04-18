cmake_minimum_required(VERSION 3.4)
project(scramble)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lGL -lglfw -lGLEW -lSOIL -Wall")

set(SOURCE_FILES
        main.cc unscramble.hh
        shader_loader.cc shader_loader.hh
        shader_linker.cc shader_linker.hh
        platform.cc platform.hh
        objects.cc objects.hh
        viewer.cc viewer.hh)

add_executable(scramble ${SOURCE_FILES})