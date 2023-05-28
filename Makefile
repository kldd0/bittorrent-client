all: clean build

clean:
	rm -r build

build:
	cmake -S . -B build && cmake --build build --config Release
