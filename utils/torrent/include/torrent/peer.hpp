#ifndef BITTORRENT_CLIENT_PEER_HPP
#define BITTORRENT_CLIENT_PEER_HPP

#include <string>

#include <boost/asio.hpp>

class Peer {
 public:
  Peer(const std::string& ip, long long port);

  void download();

 private:
  
 private:
  std::string m_ip; 
  long long m_port;
};

#endif
