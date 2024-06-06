add_executable(${PROJECT_NAME}-server
	server.cpp
)

target_include_directories(${PROJECT_NAME}-server PRIVATE ../network_lib/ ${CMAKE_BINARY_DIR}/include)

add_executable(${PROJECT_NAME}-client
	client.cpp
)

target_include_directories(${PROJECT_NAME}-client PRIVATE ../network_lib/ ${CMAKE_BINARY_DIR}/include)

