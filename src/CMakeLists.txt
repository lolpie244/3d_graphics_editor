add_library(${PROJECT_NAME}-static STATIC
	events/observer.cpp
	events/event.cpp

	events/propterties/clickable.cpp
	events/propterties/draggable.cpp
	events/propterties/hoverable.cpp
	events/propterties/scaleable.cpp
	events/propterties/scrollable.cpp

	gui/elements_container.cpp
	gui/base.cpp
	gui/sprite_element.cpp
	gui/text.cpp
	gui/button.cpp
	gui/button_list.cpp
	gui/slider.cpp
	gui/radio_button.cpp
	gui/opengl_context.cpp

	math/transform.cpp
	math/model_transform.cpp

	render/model.cpp
	render/gizmo.cpp
	render/camera.cpp
	render/light.cpp

	render/opengl/frame_buffer.cpp
	render/opengl/picking_texture.cpp
	render/opengl/vertex_array.cpp
	render/opengl/vertex_buffer.cpp

	network/connection_socket.cpp

	stage/stage.cpp
	stage/stage_manager.cpp
)
target_include_directories(${PROJECT_NAME}-static PUBLIC ../include
	${CMAKE_BINARY_DIR}/include)

target_link_libraries(${PROJECT_NAME}-static
	sfml-graphics sfml-audio
	OpenGL GLU GLEW::GLEW glm::glm
	${lunasvg} tinyobjloader::tinyobjloader alpaca::alpaca
)

