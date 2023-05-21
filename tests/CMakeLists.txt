add_executable(bencode_test bencode_test.cpp ../utils/bencode/src/bencode_parser.cpp ../utils/bencode/src/bencode_parser.cpp)
target_link_libraries(bencode_test bencode)
add_test(NAME bencode_test COMMAND bencode_test)