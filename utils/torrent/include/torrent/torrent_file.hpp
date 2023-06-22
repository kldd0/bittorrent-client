#ifndef BITTORRENT_CLIENT_TORRENT_FILE_HPP
#define BITTORRENT_CLIENT_TORRENT_FILE_HPP

#include <map>
#include <memory>
#include <vector>

#include <bencode/b_types.hpp>

class TorrentFile {
 public:
  TorrentFile(const std::string& filename);

  std::vector<unsigned char> get_info_hash();
  std::string get_announce();
  long long get_torrent_size() { return m_torrent_size; }

 private:
  void create_empty_files();

 private:
  long long m_torrent_size{0};
  std::string m_filename{};

  std::shared_ptr<BencodeObject> m_data;
};

#endif
