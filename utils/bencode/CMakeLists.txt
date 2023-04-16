add_library(bencode bencode.cpp include/bencode/bencode.hpp)
set_property(TARGET bencode PROPERTY CXX_STANDARD 20)
target_include_directories(bencode PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
install(TARGETS bencode)