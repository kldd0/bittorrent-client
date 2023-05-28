cmake_minimum_required(VERSION 3.18)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES 
	${SRC_DIR}/b_dict.cpp
	${SRC_DIR}/b_integer.cpp
	${SRC_DIR}/b_list.cpp
	${SRC_DIR}/b_object.cpp
	${SRC_DIR}/b_parser.cpp
	${SRC_DIR}/b_string.cpp
	${SRC_DIR}/b_value.cpp
)

include_directories(${INC_DIR})

add_library(bencode STATIC ${SOURCES})

set(HEADER_PATH ${INC_DIR})

target_include_directories(bencode PUBLIC ${HEADER_PATH})

set_target_properties(bencode PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

install(TARGETS bencode 
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${HEADER_PATH}/ 
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
