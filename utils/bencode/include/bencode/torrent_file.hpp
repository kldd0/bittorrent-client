#ifndef BITTORRENT_CLIENT_TORRENT_FILE_HPP
#define BITTORRENT_CLIENT_TORRENT_FILE_HPP

#include <map>
#include <memory>
#include <vector>

#include "b_types.hpp"

class TorrentFile {
 public:
  TorrentFile(const std::string& filename);

  std::string get_info_hash();
  std::string get_announce();

 private:
  void create_empty_files();

 private:
  std::string m_filename{};

  std::shared_ptr<BencodeObject> m_data;
};

#endif
