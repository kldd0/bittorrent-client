#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <string_view>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/url/parse.hpp>
#include <boost/url/src.hpp>
#include <boost/url/url_view.hpp>

#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>

#include "../../bencode/include/bencode/b_types.hpp"
#include "../include/torrent/tracker.hpp"

std::string random_string(int length) {
  const std::string CHARACTERS =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

  std::string random_string;

  for (std::size_t i = 0; i < length; ++i) {
    random_string += CHARACTERS[distribution(generator)];
  }

  return random_string;
}

std::string Tracker::generate_peer_id() {
  return "TRNT" + random_string(16);
}

std::string url_encode(const std::vector<unsigned char> & value) {
  std::ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex;

  for (auto it = value.begin(), n = value.end(); it != n; ++it) {
    std::string::value_type c = (*it);

    // Keep alphanumeric and other accepted characters intact
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      escaped << c;
      continue;
    }

    // Any other characters are percent-encoded
    escaped << std::uppercase;
    escaped << '%' << std::setw(2) << int((unsigned char)c);
    escaped << std::nouppercase;
  }

  return escaped.str();
}

void Tracker::request_peers_data() {
  namespace beast = boost::beast;  // from <boost/beast.hpp>
  namespace http = beast::http;    // from <boost/beast/http.hpp>
  namespace asio = boost::asio;    // from <boost/asio.hpp>
  namespace urls = boost::urls;
  using tcp = asio::ip::tcp;

  std::string announce_url = m_torrent_file->get_announce();

  urls::url_view uv(announce_url);

  urls::url u = uv;

  std::string host = u.host();
  std::string port = (u.port() != "") ? u.port() : "80";

  auto data = m_torrent_file->get_info_hash();
  std::string url_encoded_hash_str = url_encode(data);

  std::cout << "url_encode_str: " << url_encoded_hash_str << "\n";

  // setting query params
  u.params().append({"info_hash", url_encoded_hash_str});
  u.params().append({"peer_id", m_peer_id});
  u.params().append({"port", "6969"});
  u.params().append({"uploaded", "0"});
  u.params().append({"downloaded", "0"});
  u.params().append(
      {"left", std::to_string(m_torrent_file->get_torrent_size())});
  u.params().append({"event", "started"});

  std::string target = u.path() + "?" + u.query();

  std::cout << "host: " << host << "\n";
  std::cout << "port: " << port << "\n";
  std::cout << "target: " << target << "\n";
  std::cout << "full url: " << u << "\n";

  try {
    // The io_context is required for all I/O
    asio::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // Look up the domain name
    auto const results = resolver.resolve(host, port);

    // asio::connect(socket, resolver.resolve(host, port));
    std::cout << "results: " << results->endpoint() << "\n";

    // Make the connection on the IP address we get from a lookup
    stream.connect(results);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, target, 10};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> response;

    // Receive the HTTP response
    http::read(stream, buffer, response);

    std::string result_string =
        boost::beast::buffers_to_string(response.body().data());
    auto result_buffer =
        std::vector<unsigned char>(result_string.begin(), result_string.end());

    // parsing bencoded response from tracker
    auto b_result = std::make_shared<BencodeObject>(result_buffer);
    auto b_result_dict = b_result->get_dict();

    m_interval = std::dynamic_pointer_cast<BInteger>(
                     b_result_dict->get_value().find("interval")->second)
                     ->get_value();

    // check if peers data have already unpacked
    if (auto p = std::dynamic_pointer_cast<BList>(
            b_result_dict->get_value().find("peers")->second)) {

      auto peers_list = std::dynamic_pointer_cast<BList>(
                            b_result_dict->get_value().find("peers")->second)
                            ->get_value();

      // interating over peers dict
      for (auto const& peer_dict : peers_list) {
        // getting peer ip
        std::string peer_ip = std::dynamic_pointer_cast<BString>(
                                  std::dynamic_pointer_cast<BDict>(peer_dict)
                                      ->get_value()
                                      .find("ip")
                                      ->second)
                                  ->get_value();
        // getting peer port
        long long peer_port = std::dynamic_pointer_cast<BInteger>(
                                  std::dynamic_pointer_cast<BDict>(peer_dict)
                                      ->get_value()
                                      .find("port")
                                      ->second)
                                  ->get_value();
        // adding to dict
        m_peers_ep.emplace_back(
            tcp::endpoint(asio::ip::address::from_string(peer_ip), peer_port));
      }
    } else {
      auto peers_bytes = std::dynamic_pointer_cast<BBytes>(
          b_result_dict->get_value().find("peers")->second);

      // getting indexes of peers bytes section in all buffer
      int start_pos = peers_bytes->get_start_pos();
      int end_pos = peers_bytes->get_end_pos();

      // creating new vector of bytes only with peers bytes
      auto peer_bytes_buff = std::vector<unsigned char>(
          b_result->get_buffer().begin() + start_pos,
          b_result->get_buffer().begin() + end_pos + 1);

      // parsing compressed format of peers
      // 6 bytes for every ip address, 4b - ip in ipv4 format, 2b - port
      for (auto it = peer_bytes_buff.begin(); it != peer_bytes_buff.end();
           it += 6) {
        std::string peer_ip = "";
        for (auto c = it; c != it + 4; ++c) {
          peer_ip += std::to_string(*c);
          if (c != it + 3) {
            peer_ip += ".";
          }
        }
        long long peer_port = (*(it + 4)) * 256 + (*(it + 5));
        m_peers_ep.emplace_back(
            tcp::endpoint(asio::ip::address::from_string(peer_ip), peer_port));
      }
    }

    // Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes
    if (ec && ec != beast::errc::not_connected) {
      throw beast::system_error{ec};
    }
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void Tracker::check_peers() {
  std::cout << "Peers count: " << m_peers_ep.size() << "\n";

  try {
    boost::asio::io_service io_service;

    std::vector<std::shared_ptr<Peer>> peers;
    int p_count = 1;
    for (auto& ep : m_peers_ep) {
      /*
      peers.emplace_back(
          std::make_shared<Peer>(p_count, m_peer_id, ep, io_service, m_torrent_file));
      ++p_count;
      */
      tcp::endpoint edp(asio::ip::address::from_string("31.22.89.111"), 6897);
      peers.emplace_back(
          std::make_shared<Peer>(
          p_count, m_peer_id,
          edp, io_service,
          m_torrent_file));
      break;
    }

    // start async operations
    io_service.run();
  } catch (std::exception& e) {
    std::cerr << "Exception [check_peers]: " << e.what() << "\n";
  }
}

void Tracker::unpack_peer_bytes() {}

Tracker::Tracker(std::shared_ptr<TorrentFile> torrent)
    : m_peer_id(generate_peer_id()), m_torrent_file(torrent) {

  std::cout << "peer_id: " << m_peer_id << " byte size: " << sizeof(m_peer_id)
            << "\n";
  std::cout << "announce: " << m_torrent_file->get_announce() << "\n";

  request_peers_data();

  check_peers();
}
