cmake_minimum_required(VERSION 3.10)
project(gsh C)

set(CMAKE_C_STANDARD 99)

file(GLOB SOURCES "src/*.c")

add_executable(gsh ${SOURCES})
