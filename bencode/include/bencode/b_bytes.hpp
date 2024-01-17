#ifndef BITTORRENT_CLIENT_BBYTES_HPP
#define BITTORRENT_CLIENT_BBYTES_HPP

#include <memory>
#include <string>
#include <utility>

#include "b_type.hpp"

/**
 * @brief Class bytes
 *
 * Class representing raw bytes (stored in pieces key) in bencoding
 * instance of this type just store indexes of raw byte sequence
 */
class BBytes : public BType {
 public:
  /**
   * @brief Construct a new BBytes object
   * 
   * @param pair_of_indexes Indexes of starting and ending position in raw byte buffer
   */
  explicit BBytes(std::pair<int, int> pair_of_indexes);

  /**
   * @brief Get the type object
   * 
   * @return const std::string& 
   */
  [[nodiscard]] const std::string& get_type() const override { return m_type; }

  /**
   * @brief Get the start pos object
   * 
   * @return const int& Starting index in buffer
   */
  const int& get_start_pos() const { return m_start_pos; }
  /**
   * @brief Get the end pos object
   * 
   * @return const int& Ending index in buffer 
   */
  const int& get_end_pos() const { return m_end_pos; }

 private:
  int m_start_pos{0};
  int m_end_pos{0};

  std::string m_type{"bytes"}; // typename value used for upcast
};

#endif
