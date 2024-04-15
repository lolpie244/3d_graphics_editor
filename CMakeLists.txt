cmake_minimum_required(VERSION 3.28)

project(3d_graphics CXX)

find_package(SFML 2.6.1 REQUIRED COMPONENTS graphics window system audio)

include(ExternalProject)
ExternalProject_Add(lunasvg-git
	GIT_REPOSITORY https://github.com/lolpie244/lunasvg
	GIT_TAG master
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}
)

set(lunasvg ${CMAKE_BINARY_DIR}/lib/liblunasvg.a)

add_subdirectory(src)
add_subdirectory(app)
