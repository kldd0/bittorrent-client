#include <iostream>
#include <string>

#include "utils/bencode_parser/include/bencode_parser/bencode_parser.hpp"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "You didn't specify the path to the file" << "\n";
    return 0;
  }
  std::string filename(argv[1]);
  auto test = BencodeParser(filename);
  return 0;
}
