all: cmake

cmake:
	[ -f build/CMakeCache.txt ] && [ "$$(pwd)" != "$$(grep 'CMAKE_HOME_DIRECTORY:INTERNAL' build/CMakeCache.txt | cut -d '=' -f 2)" ] \
&& rm -rf build || true
	mkdir -p build && cd build && cmake ..
	make -C build

clean:
	rm -rf build