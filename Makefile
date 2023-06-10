all: clean build

clean:
	rm -r build

debug:
	cmake --debug-output -S . -B build

build:
	cmake -S . -B build && cmake --build build --config Release
