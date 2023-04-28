add_library(bencode_parser bencode_parser.cpp include/bencode_parser/bencode_parser.hpp)
set_property(TARGET bencode_parser PROPERTY CXX_STANDARD 20)
target_include_directories(bencode_parser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
install(TARGETS bencode_parser)
