cmake_minimum_required(VERSION 3.28)

project(3d_graphics CXX)

find_package(SFML 2.6.1 REQUIRED COMPONENTS graphics window system audio)

add_library(${PROJECT_NAME}-static STATIC
	src/events/observer.cpp

	src/gui/elements/base.cpp
	src/gui/elements/button.cpp
	src/gui/elements/slider.cpp
	src/gui/elements/radio_button.cpp

	src/stage/stage.cpp
	src/stage/stage_manager.cpp

	src/utils/utils.cpp
)
target_include_directories(${PROJECT_NAME}-static PUBLIC include)
target_link_libraries(${PROJECT_NAME}-static sfml-graphics sfml-audio)


add_executable(${PROJECT_NAME}
	app/main.cpp
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static)
