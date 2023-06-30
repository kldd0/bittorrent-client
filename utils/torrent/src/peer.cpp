#include <bitset>
#include <exception>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/beast/core.hpp>
#include <boost/bind/bind.hpp>
#include <boost/endian/conversion.hpp>

#include "../include/torrent/peer.hpp"

/**
 * @brief Message enum type 
 *
 * Enum class for convenient storage of message types
 */
enum class Message_type : int {
  choke = 0,
  unchoke = 1,
  interested = 2,
  not_interested = 3,
  have = 4,
  bitfield = 5,
  request = 6,
  piece = 7,
  cancel = 8
};

std::string out_bits(const std::vector<bool>& bits);

void Peer::process_bitfield(std::vector<unsigned char>::iterator s,
                            std::vector<unsigned char>::iterator e) {
  long long pieces_count = m_torrent_file->get_pieces_count();
  m_bitf_pieces.reserve(pieces_count);

  for (auto it = s + 1; it != e; ++it) {
    std::bitset<8> bits(*it);
    for (int i = 7; i >= 0; --i) {
      if (m_bitf_pieces.size() >= pieces_count)
        break;
      m_bitf_pieces.push_back(bits[i]);
    }
  }
}

void Peer::handle_response(const boost::system::error_code& err,
                           std::size_t bytes_transferred) {
  // reader pointer init
  auto response_it = m_response.begin();

  // base logging info
  std::string m_base_log = "Peer[" + m_ip + ":" + std::to_string(m_port) + "]";

  if (!err || err == asio::error::eof) {
    // if the message is handshake
    if (!m_is_got_handshake && bytes_transferred != 0) {
      std::string resp_info =
          "Received " + std::to_string(bytes_transferred) +
          " bytes from peer №" + std::to_string(m_id) + " " + m_base_log +
          " Got handshake; peer id - [" +
          std::string(response_it + 48, response_it + m_handshake_length) +
          "]\n";
      std::cout << resp_info;

      // moving reader pointer to end of handshake position
      response_it += m_handshake_length;
      m_is_got_handshake = true;
    }

    while (response_it < m_response.begin() + bytes_transferred) {
      std::cout << "loop\n";
      // if nothing has been transmitted
      if (bytes_transferred == 0) {
        std::string info = m_base_log + " No messages. Closing connection\n";
        std::cout << info;

        // closing socket
        m_socket.close();
        return;
      }

      // getting first 4 bytes for getting length of message
      std::vector<unsigned char> integer_buff(response_it, response_it + 4);
      int byte_length = boost::endian::load_big_u32(integer_buff.data());
      response_it += 4;

      std::cout << m_base_log + " Byte length of message: " +
                       std::to_string(byte_length) + "\n";

      if (byte_length == 0) {
        std::cout << m_base_log + " Message: keep alive\n";
        // keep reading
        continue;
      }

      std::string message;
      try {
        message = std::string(response_it, response_it + byte_length);
        response_it += byte_length;
      } catch (std::exception& e) {
        std::cout << m_base_log +
                         " Message: Can't read message; Closing connection\n";
        m_socket.close();
        return;
      }

      // message_id -> 1st byte of message
      int message_id = message[0];

      if (message_id == static_cast<int>(Message_type::choke)) {
        std::cout << m_base_log + " Message: CHOKE\n";
        m_choked = true;
      } else if (message_id == static_cast<int>(Message_type::unchoke)) {
        std::cout << m_base_log + " Message: UNCHOKE\n";
        m_choked = false;
      } else if (message_id == static_cast<int>(Message_type::interested)) {
        std::cout << m_base_log + " Mmessage: INTERESTED\n";
      } else if (message_id == static_cast<int>(Message_type::not_interested)) {
        std::cout << m_base_log + " Message: NOT_INTERESTED\n";
      } else if (message_id == static_cast<int>(Message_type::have)) {
        std::cout << m_base_log + " Message: HAVE\n";
      } else if (message_id == static_cast<int>(Message_type::bitfield)) {
        process_bitfield(response_it - byte_length, response_it);
        std::string bits = out_bits(m_bitf_pieces);
        std::cout << m_base_log + " Message: BITFIELD | size = " +
                         std::to_string(m_bitf_pieces.size()) + " " + bits +
                         "\n";

        // sending message that we are interested in transfer
        std::vector<unsigned char> request = {0x00, 0x00, 0x00, 0x01, 0x02};
        asio::async_write(
            m_socket, asio::buffer(request, 5),
            boost::bind(&Peer::handle_write, this, boost::placeholders::_1,
                        boost::placeholders::_2));
        return;
      } else if (message_id == static_cast<int>(Message_type::request)) {
        std::cout << m_base_log + " Message: REQUEST\n";
      } else if (message_id == static_cast<int>(Message_type::piece)) {
        std::cout << m_base_log + " Message: PIECE\n";
      } else if (message_id == static_cast<int>(Message_type::cancel)) {
        std::cout << m_base_log + " Message: CANCEL\n";
      } else {
        std::cout << m_base_log + " Message: UKNOWN ID[" +
                         std::to_string(message_id) + "]\n";
      }

      // if ((m_cur_piece_id == -1) && (!m_choked)) {
      //   request_piece();
      //   // nothing to download; disconnect
      // }
    }
    std::cout << "out from loop\n";

    // keep reading another message
    m_socket.async_read_some(
        asio::buffer(m_response),
        boost::bind(&Peer::handle_response, this, boost::placeholders::_1,
                    boost::placeholders::_2));
  } else {
    std::string error_rec =
        "error receiving data from " + m_base_log + " " + err.message() + "\n";
    std::cerr << error_rec;

    // closing socket
    m_socket.close();
  }
}

void Peer::handle_write(const boost::system::error_code& err,
                        std::size_t bytes_transferred) {
  if (!err) {
    std::string req_status =
        "Handshake sent: " + std::to_string(bytes_transferred) + " bytes" +
        "\n";
    std::cout << req_status;

    // wait for response
    // asynchronous reading for response
    m_socket.async_read_some(
        asio::buffer(m_response),
        boost::bind(&Peer::handle_response, this, boost::placeholders::_1,
                    boost::placeholders::_2));
  } else {
    std::string err_send = "Error sending handshake to peer [" + m_ip + ":" +
                           std::to_string(m_port) + "] " + err.message() +
                           "; Closing connection\n";
    std::cerr << err_send;

    // closing socket
    m_socket.close();
  }
}

void Peer::handle_connect(const boost::system::error_code& err) {
  if (!err) {
    // if connection is established
    std::string status = "Peer[" + m_ip + ":" + std::to_string(m_port) + "] " +
                         "Connection established\n";
    std::cout << status;

    // Send the handshake
    std::vector<unsigned char> request = handshake();
    asio::async_write(
        m_socket, asio::buffer(request, m_handshake_length),
        boost::bind(&Peer::handle_write, this, boost::placeholders::_1,
                    boost::placeholders::_2));
  } else {
    std::string err_conn = "Error connecting to peer [" + m_ip + ":" +
                           std::to_string(m_port) + "] " + err.message() + "\n";
    std::cerr << err_conn;

    // closing socket
    m_socket.close();
  }
}

std::vector<unsigned char> Peer::handshake() {
  std::vector<unsigned char> m;
  m.push_back(19);
  const char* str = "BitTorrent protocol";
  m.insert(m.end(), str, str + 19);
  m.insert(m.end(), 8, '\0');
  auto info_hash_bytes = m_torrent_file->get_info_hash();
  m.insert(m.end(), info_hash_bytes.begin(), info_hash_bytes.end());
  const char* peer_str = m_peer_id.data();
  m.insert(m.end(), peer_str, peer_str + 20);
  return m;
}

std::string out_bits(const std::vector<bool>& bits) {
  std::string s = "";
  for (auto e : bits) {
    s += std::to_string(e);
  }
  return s;
}

Peer::Peer(int id, const std::string& peer_id, tcp::endpoint& ep,
           asio::io_service& ios, std::shared_ptr<TorrentFile> torrent_file)
    : m_id(id),
      m_peer_id(peer_id),
      m_socket(ios),
      m_ip(ep.address().to_string()),
      m_port(ep.port()),
      m_endpoint(ep),
      m_torrent_file(torrent_file),
      m_response(1024) {
  m_socket.open(tcp::v4());
  m_socket.set_option(boost::asio::ip::tcp::socket::reuse_address(true));

  // asynchronous connect
  m_socket.async_connect(m_endpoint, boost::bind(&Peer::handle_connect, this,
                                                 boost::placeholders::_1));
}
