#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>

#include "include/bencode_parser/bencode_parser.hpp"

BencodeParser::BencodeParser(std::string &filename)
  : m_filename(filename)
{
  readFile();
};

void BencodeParser::readFile()
{
  std::ifstream input(m_filename, std::ios::binary);
  std::stringstream sstr;

  std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
  std::vector<unsigned char> parser;
  for (uint32_t i = 0; i < buffer.size(); ++i) {
    
    unsigned char character = buffer[i];
    parser.push_back(character);
  }
  std::cout << buffer.size() << "\n";
}
