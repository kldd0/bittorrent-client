cmake_minimum_required(VERSION 3.7)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES 
	${SRC_DIR}/torrent_file.cpp
	${SRC_DIR}/tracker.cpp
	${SRC_DIR}/peer.cpp
)

include_directories(${INC_DIR})

add_library(torrent STATIC ${SOURCES})

target_link_libraries(torrent PUBLIC bencode)

set(HEADER_PATH ${INC_DIR})

target_include_directories(torrent PUBLIC ${HEADER_PATH})

set_target_properties(torrent PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

install(TARGETS torrent 
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${HEADER_PATH}/ 
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
