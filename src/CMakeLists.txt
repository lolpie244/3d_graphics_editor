add_library(${PROJECT_NAME}-static STATIC
	events/observer.cpp
	events/event.cpp

	gui/elements/base.cpp
	gui/elements/sprite_element.cpp
	gui/elements/text.cpp
	gui/elements/button.cpp
	gui/elements/slider.cpp
	gui/elements/radio_button.cpp

	gui/propteries/clickable.cpp
	gui/propteries/draggable.cpp
	gui/propteries/hoverable.cpp
	gui/propteries/scaleable.cpp

	stage/stage.cpp
	stage/stage_manager.cpp

	utils/utils.cpp
)
target_include_directories(${PROJECT_NAME}-static PUBLIC ../include)
target_link_libraries(${PROJECT_NAME}-static sfml-graphics sfml-audio)

