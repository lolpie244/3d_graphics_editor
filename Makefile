BUILD_TYPE ?= Debug

.PHONY: openal_fix
openal_fix:
	git clone https://github.com/conan-io/conan-center-index.git
	cd conan-center-index;\
	git fetch origin pull/21791/head:pr21791;\
	git switch pr21791;\
	conan create recipes/openal-soft/all --version=1.22.2 --build=libalsa/1.2.10 --build=openal-soft/1.22.2
	rm -rf conan-center-index

.PHONY: setup
setup:
	conan install . -pr=./conan_profile --output-folder=build --build=missing -s build_type=${BUILD_TYPE}

############### BUILD ###############
.PHONY: build
build:
	cd build;\
	cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_EXPORT_COMPILE_COMMANDS=1;\
	cmake --build . --config ${BUILD_TYPE}

.PHONY: run
run: build
	build/app/3d_graphics
