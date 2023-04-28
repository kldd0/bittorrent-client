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
    void readFile();

  private:
    // std::unique_ptr<BencodeObject>
    std::string m_filename {};
};

#endif //BITTORRENT_CLIENT_BENCODE_PARSER_HPP
