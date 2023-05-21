#ifndef BITTORRENT_CLIENT_BENCODE_PARSER_HPP
#define BITTORRENT_CLIENT_BENCODE_PARSER_HPP

#include <map>
#include <memory>
#include <string>

class BencodeParser
{
public:
  explicit BencodeParser(std::string& filename);

private:
  void parseFile();

private:
  std::string m_filename {};
  // std::unique_ptr<BencodeObject>
};

#endif //BITTORRENT_CLIENT_BENCODE_PARSER_HPP
