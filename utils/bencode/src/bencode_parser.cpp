#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>

#include "../include/bencode_parser.hpp"

void ParseStruct(const std::vector<unsigned char>& buffer)
{
  auto it = buffer.begin(); // iterator to character from buffer
  while (it != buffer.end()) {
    switch (*it) {
      case 'd':
        std::cout << "found d";
      default:
        break;
    }
    ++it;
  }
}

BencodeParser::BencodeParser(std::string &filename)
  : m_filename(filename)
{
  parseFile();
};

void BencodeParser::parseFile()
{
  std::ifstream input(m_filename, std::ios::binary);
  std::stringstream sstr;

  std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
  ParseStruct(buffer);
  std::cout << buffer.size() << "\n";
}
