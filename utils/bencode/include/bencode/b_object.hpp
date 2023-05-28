#ifndef BITTORRENT_CLIENT_BENCODE_OBJECT_HPP
#define BITTORRENT_CLIENT_BENCODE_OBJECT_HPP

#include <map>
#include <string>
#include <memory>

#include "b_value.hpp"

class BencodeObject
{
  public:

  private:
    std::map<std::string, std::unique_ptr<BValue>> m_dict;
};

#endif
