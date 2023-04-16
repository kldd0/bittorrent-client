#include <fstream>
#include <sstream>
#include <iostream>

#include "include/bencode/bencode.hpp"

Bencode::Bencode(std::string &filename)
  : m_filename(filename)
{
  readFile();
};

void Bencode::readFile()
{
  std::ifstream input(m_filename);
  std::stringstream sstr;

  while (input >> sstr.rdbuf());

  std::cout << sstr.str() << "\n";
}
