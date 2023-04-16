#include <iostream>
#include <string>

#include "utils/bencode/include/bencode/bencode.hpp"

int main(int argc, char **argv) {
  if (argc < 1) {
    std::cout << "You didn't specify the path to the file";
    return 0;
  }
  std::string filename(argv[1]);
  auto test = Bencode(filename);
  return 0;
}
