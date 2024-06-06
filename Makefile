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
	mkdir -p build/include
	mkdir -p build/lib
	mkdir -p build/lib
	conan install . -pr=./conan_profile --output-folder=build --build=missing -s build_type=${BUILD_TYPE}

	wget -O build/tinyfiledialogs.zip https://sourceforge.net/projects/tinyfiledialogs/files/latest/download
	unzip build/tinyfiledialogs.zip -d build/
	cd build/tinyfiledialogs; \
	gcc -fPIC -c tinyfiledialogs.c -o tinyfiledialogs.o; \
	gcc -shared -o libtinyfiledialogs.so tinyfiledialogs.o

	cp build/tinyfiledialogs/tinyfiledialogs.h build/include
	cp build/tinyfiledialogs/libtinyfiledialogs.so build/lib


############### BUILD ###############
.PHONY: build
build:
	cd build;\
	cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_EXPORT_COMPILE_COMMANDS=1;\
	cmake --build . --config ${BUILD_TYPE}

.PHONY: run
run: build
	prime-run build/app/3d_graphics

.PHONY: run-intel
run-intel: build
	build/app/3d_graphics

TYPES := server client
.PHONY: run-${type}
$(TYPES:%=run-%): build
	build/server/3d_graphics-$(@:run-%=%)


.PHONY: format
format:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -style=file -i {} \;
