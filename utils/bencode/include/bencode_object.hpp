#ifndef BITTORRENT_CLIENT_BENCODE_OBJECT_HPP
#define BITTORRENT_CLIENT_BENCODE_OBJECT_HPP

#include <map>
#include <string>

class BItem {};

class BencodeObject
{
  public:

  private:
    std::map<std::string, *BItem> m_dictionary;
};

#endif // BITTORRENT_CLIENT_BENCODE_OBJECT_HPP 
