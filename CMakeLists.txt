cmake_minimum_required(VERSION 3.28)

project(3d_graphics CXX)

find_package(SFML 2.6.1 REQUIRED COMPONENTS graphics window system audio)

add_subdirectory(src)
add_subdirectory(app)
