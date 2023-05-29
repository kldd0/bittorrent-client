#ifndef BITTORRENT_CLIENT_BENCODE_PARSER_HPP
#define BITTORRENT_CLIENT_BENCODE_PARSER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "b_value.hpp"

class BencodeParser
{
public:
  explicit BencodeParser(std::string& filename);
  
  void parse_file();

private:

  int get_index_of_closing_pair(std::pair<int, unsigned char> p);
  void parse(std::unique_ptr<BValue>& b_object, const std::vector<unsigned char>& buffer, int& index, int& end);

private:
  std::string m_filename {};
  std::vector<std::pair<int, unsigned char>> m_pairs;
  // std::unique_ptr<BencodeObject>
};

#endif
