cmake_minimum_required(VERSION 3.7)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
	${SOURCE_DIR}/b_dict.cpp
	${SOURCE_DIR}/b_integer.cpp
	${SOURCE_DIR}/b_list.cpp
	${SOURCE_DIR}/b_object.cpp
	${SOURCE_DIR}/b_bytes.cpp
	${SOURCE_DIR}/b_string.cpp
)

include_directories(${INCLUDE_DIR})

add_library(bencode ${SOURCES})

set(HEADER_PATH ${INCLUDE_DIR})

target_include_directories(bencode PUBLIC ${HEADER_PATH})

set_target_properties(bencode PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# install(TARGETS bencode
#     ARCHIVE DESTINATION lib
# )

install(DIRECTORY ${HEADER_PATH}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
