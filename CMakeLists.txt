cmake_minimum_required(VERSION 3.7)
project(bittorrent_client)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_VERBOSE_MAKEFILE True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

add_subdirectory(utils)
add_subdirectory(tests)

# if(Boost_FOUND)
#     include_directories(${Boost_INCLUDE_DIRS}) 
#     add_executable(bittorrent_client main.cpp) 
#     target_link_libraries(bittorrent_client bencode ${Boost_LIBRARIES})
# endif()

add_executable(bittorrent_client main.cpp)

target_link_libraries(bittorrent_client torrent)
