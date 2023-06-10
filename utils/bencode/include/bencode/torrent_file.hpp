#ifndef BITTORRENT_CLIENT_TORRENT_FILE_HPP
#define BITTORRENT_CLIENT_TORRENT_FILE_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "b_types.hpp"

class TorrentFile
{
public:
  explicit TorrentFile(std::string& filename);

  void parse_file();
  
private:
  // helper function for decoding algorithm
  int get_index_of_closing_pair (std::pair<int, unsigned char> p);

  // main parsing function
  void parse(std::shared_ptr<BType>& b_object, const std::vector<unsigned char>& buffer, int& index, int& end);

  std::string get_info_hash();

  // function for creating empty files
  void create_empty_files();

private:
  std::string m_filename {};
  std::vector<unsigned char> m_buffer;
  std::vector<std::pair<int, unsigned char>> m_pairs;

  std::shared_ptr<BDict> m_data;
};

#endif
