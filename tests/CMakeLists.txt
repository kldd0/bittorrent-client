add_executable(bencode_test bencode_test.cpp)
target_link_libraries(bencode_test bencode)
add_test(NAME bencode_test COMMAND bencode_test)
