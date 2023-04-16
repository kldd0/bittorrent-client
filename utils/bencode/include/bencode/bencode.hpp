//
// Created by Reshetnyak Stepan on 16.04.2023.
//

#ifndef BITTORRENT_CLIENT_BENCODE_HPP
#define BITTORRENT_CLIENT_BENCODE_HPP

#include <string>

class Bencode
{
  public:
      explicit Bencode(std::string& filename);

  private:
      void readFile();

  private:
      std::string m_filename {};
};

#endif //BITTORRENT_CLIENT_BENCODE_HPP
