add_library(bencode
	src/bencode_parser.cpp src/bencode_object.cpp
	include/bencode_parser.hpp include/bencode_object.hpp
)
set_property(TARGET bencode PROPERTY CXX_STANDARD 20)
target_include_directories(bencode PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
install(TARGETS bencode)