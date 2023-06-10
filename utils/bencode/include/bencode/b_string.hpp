#ifndef BITTORRENT_CLIENT_B_STRING_HPP
#define BITTORRENT_CLIENT_B_STRING_HPP

#include <string>

#include "b_type.hpp"

/**
 * Class representing string in bencoding
 */
class BString : public BType
{
public:
  explicit BString(std::string str);

  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  [[nodiscard]] const std::string& get_value() const { return m_value; }

private:
  std::string m_type {"string"};
  std::string m_value {};
};

#endif
