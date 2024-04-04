add_library(${PROJECT_NAME}-static STATIC
	events/observer.cpp
	events/event.cpp

	gui/elements/base.cpp
	gui/elements/button.cpp
	gui/elements/slider.cpp
	gui/elements/radio_button.cpp

	stage/stage.cpp
	stage/stage_manager.cpp

	utils/utils.cpp
)
target_include_directories(${PROJECT_NAME}-static PUBLIC ../include)
target_link_libraries(${PROJECT_NAME}-static sfml-graphics sfml-audio)

