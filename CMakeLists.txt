cmake_minimum_required(VERSION 3.28)

project(3d_graphics CXX)

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include(ExternalProject)
ExternalProject_Add(lunasvg-git
	GIT_REPOSITORY https://github.com/lolpie244/lunasvg
	GIT_TAG master
	UPDATE_COMMAND ""
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}
)

set(lunasvg ${CMAKE_BINARY_DIR}/lib/liblunasvg.a)
set(tinyfiledialogs ${CMAKE_BINARY_DIR}/lib/libtinyfiledialogs.so)

find_package(SFML 2.6.1 REQUIRED COMPONENTS graphics window system audio)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(alpaca REQUIRED)

add_subdirectory(src)
add_subdirectory(app)
