#ifndef BITTORRENT_CLIENT_TRACKER_HPP
#define BITTORRENT_CLIENT_TRACKER_HPP

#include <string>

#include "peer.hpp"
#include "torrent_file.hpp"

using boost::asio::ip::tcp;

/**
 * @brief Class tracker 
 * 
 * Class representing the entity of tracker that control
 * peer network interaction
 */
class Tracker {
 public:
  /**
   * @brief Construct a new Tracker object
   * 
   * @param torrent Pointer to a torrent file object
   */
  Tracker(std::shared_ptr<TorrentFile> torrent);

  /**
   * @brief Function for making request to a main tracker
   * for getting information about peers (their ip and port) 
   */
  void request_peers_data();

 private:
  /**
   * @brief Function for generating self id
   * 
   * @return std::string 
   */
  std::string generate_peer_id();

  /**
   * @brief Function for unpacking raw bytes format 
   * of returned peers data 
   */
  void unpack_peer_bytes();

  /**
   * @brief Main function for sending handshakes
   * and receive messages from peers 
   */
  void check_peers();

 private:
  std::string m_peer_id; // peer id value

  long long m_interval; // timeout interval for sending requests to peer

  std::vector<tcp::endpoint> m_peers_ep; // vector of peers endpoints

  std::shared_ptr<TorrentFile> m_torrent_file; // pointer to torrent file 
};

#endif
