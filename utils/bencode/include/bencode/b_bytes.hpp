#ifndef BITTORRENT_CLIENT_BBYTES_HPP
#define BITTORRENT_CLIENT_BBYTES_HPP

#include <memory>
#include <string>
#include <utility>

#include "b_type.hpp"

/**
 * Class representing raw bytes (stored in pieces key) in bencoding
 * instance of this type just store indexes of raw byte sequence
 */
class BBytes : public BType
{
public:
  explicit BBytes(std::pair<int, int> pair_of_indexes);

  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  
  const int& get_start_pos() const { return m_start_pos; }
  const int& get_end_pos()   const { return m_end_pos; }

private:
  int m_start_pos {0};
  int m_end_pos {0};

  std::string m_type {"bytes"};
};

#endif
