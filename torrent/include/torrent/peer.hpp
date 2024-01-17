#ifndef BITTORRENT_CLIENT_PEER_HPP
#define BITTORRENT_CLIENT_PEER_HPP

#include <string>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>

#include "./torrent_file.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

/**
 * @brief Class peer
 *
 * Class representing the entity of one peer node
 * as a participant in network interaction
 */
class Peer {
 public:
  /**
   * @brief Construct a new Peer object
   *
   * @param id Peer number (just for easy observation in output)
   * @param peer_id Individual peer id generated by tracker object
   * @param ep Endpoing boost type object (pair of ip and port)
   * @param ios I/O service object
   * @param torrent_file Pointer to torrent file object
   */
  Peer(int id, std::string  peer_id, tcp::endpoint& ep,
       asio::io_service& ios, std::shared_ptr<TorrentFile> torrent_file);

 private:
  /**
   * @brief Generating handshake function
   *
   * @return std::vector<unsigned char>
   */
  std::vector<unsigned char> handshake();

  /**
   * @brief Callback function for handling connection
   *
   * @param err Returned error code
   */
  void handle_connect(const boost::system::error_code& err);

  /**
   * @brief Callback function for handling socket write operation
   *
   * @param err Returned error code
   * @param bytes_transferred Number of transmitted bytes
   */
  void handle_write(const boost::system::error_code& err,
                    std::size_t bytes_transferred);

  /**
   * @brief Callback function for handling response from peer
   *
   * @param err Returned error code
   * @param bytes_transferred Number of transmitted bytes
   */
  void handle_response(const boost::system::error_code& err,
                       std::size_t bytes_transferred);

  /**
   * @brief Helper debug function for displaying the content
   * of the value of bitfield message
   *
   * @param s Starting buffer iterator
   * @param e Ending buffer iterator
   */
  void process_bitfield(std::vector<unsigned char>::iterator s,
                        std::vector<unsigned char>::iterator e);

 private:
  // for statistics
  int m_id{0};

  // message status
  std::string m_status = "length";

  // tracker generated id
  std::string m_peer_id{};

  // network
  std::string m_ip;
  long long m_port;

  tcp::socket m_socket;
  tcp::endpoint m_endpoint;

  // buffer
  std::vector<unsigned char> m_response;

  // bitfield
  std::vector<bool> m_bitf_pieces;

  // handshake status
  bool m_is_got_handshake{false};
  int m_handshake_length = 68;

  bool m_choked{false};

  // current downloading piece id
  int m_cur_piece_id{-1};

  std::shared_ptr<TorrentFile> m_torrent_file;
};

#endif