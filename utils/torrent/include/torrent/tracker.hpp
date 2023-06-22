#ifndef BITTORRENT_CLIENT_TRACKER_HPP
#define BITTORRENT_CLIENT_TRACKER_HPP

#include <string>

#include "peer.hpp"
#include "torrent_file.hpp"

class Tracker {
 public:
  Tracker(std::unique_ptr<TorrentFile> torrent);
  void request_peers_data();

 private:
  std::string generate_peer_id();
  void check_peers();

 private:
  std::string m_peer_id; 
  long long m_interval;

  std::vector<std::shared_ptr<Peer>> m_peers; 

  std::unique_ptr<TorrentFile> m_torrent_file;
};

#endif

