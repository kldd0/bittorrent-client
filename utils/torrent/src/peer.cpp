#include <future>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>

#include "../include/torrent/peer.hpp"

Peer::Peer(const std::string& ip, long long port) : m_ip(ip), m_port(port) {}

void Peer::download() {
  namespace asio = boost::asio;
  using tcp = asio::ip::tcp;

  // I/O context
  asio::io_context io_context;

  tcp::socket socket(io_context);
  tcp::endpoint ep(asio::ip::address::from_string(m_ip), m_port);

  // asynchronous connect
  socket.open(tcp::v4());
  socket.set_option(boost::asio::ip::tcp::socket::reuse_address(true));
  socket.async_connect(
      ep, [this](const boost::system::error_code& error) {
        if (!error) {
          std::cout << "Peer[" << m_ip << ":" << m_port << "] : "
                    << "Connection successful"
                    << "\n";
        } else {
          std::cout << "Peer[" << m_ip << ":" << m_port << "] : "
                    << "Connection failed"
                    << "\n";
        }
      });

  // create a timer to wait for the connection to complete with a timeout value
  boost::asio::steady_timer timer(io_context,
                                  boost::asio::chrono::milliseconds(500));
  timer.async_wait([&socket, this](const boost::system::error_code& error) {
    if (!error) {
      std::cout << "Peer[" << m_ip << ":" << m_port << "] : "
                << "Connection timeout"
                << "\n";
      socket.close();
    }
  });

  io_context.run();
}
