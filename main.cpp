#include <iostream>
#include <string>
#include <fstream>

#include <bencode/b_parser.hpp>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "You didn't specify the path to the file" << "\n";
    return 0;
  }
  std::string filename(argv[1]);

  std::ifstream input(filename, std::ios::binary);

  auto test = BencodeParser(filename);
  test.parse_file();

  return 0;
}
