#ifndef BITTORRENT_CLIENT_TORRENT_FILE_HPP
#define BITTORRENT_CLIENT_TORRENT_FILE_HPP

#include <map>
#include <memory>
#include <vector>

#include <bencode/b_types.hpp>

/**
 * @brief Class torrent file 
 * 
 * Class representing the entity of torrent file
 */
class TorrentFile {
 public:
  /**
   * @brief Construct a new Torrent File object
   * 
   * @param filename Relative file path
   */
  TorrentFile(const std::string& filename);

  /**
   * @brief Get the announce url
   * 
   * @return std::string 
   */
  std::string get_announce();

  /**
   * @brief Get the sha1 hash bytes of info dictionary
   * 
   * @return std::vector<unsigned char> 
   */
  std::vector<unsigned char> get_info_hash();
  /**
   * @brief Get sha1 hash bytes of piece
   * 
   * @param i Index of piece
   * @return std::vector<unsigned char> 
   */
  std::vector<unsigned char> get_piece_hash(int i);

  /**
   * @brief Get the torrent size value
   * 
   * @return long long 
   */
  long long get_torrent_size() { return m_torrent_size; }
  /**
   * @brief Get the piece length
   * 
   * @return long long 
   */
  long long get_piece_length() { return m_piece_length; }
  /**
   * @brief Get the pieces count value
   * 
   * @return long long 
   */
  long long get_pieces_count() { return m_pieces_count; }

 private:
  /**
   * @brief Function for creating file structure of the loaded entity
   */
  void create_empty_files();
  /**
   * @brief Function for preparing piece_length & pieces_count
   */
  void process_piece_length_and_pieces_count();

 private:
  long long m_torrent_size{0}; // torrent size value
  long long m_piece_length{0}; // piece length value
  long long m_pieces_count{0}; // pieces count value

  std::string m_filename{};

  std::shared_ptr<BencodeObject> m_data; // pointer to bencode object
};

#endif
