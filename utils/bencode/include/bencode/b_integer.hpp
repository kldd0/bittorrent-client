#ifndef BITTORRENT_CLIENT_B_INTEGER_HPP
#define BITTORRENT_CLIENT_B_INTEGER_HPP

#include "b_type.hpp"

/**
 * Class representing numerical type in bencoding
 * type is long long because values can be huge
 */
class BInteger : public BType
{
public:
  explicit BInteger(long long num);

  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  [[nodiscard]] long long get_value() const { return m_value; }

private:
  std::string m_type {"int"};
  long long m_value {0};
};

#endif
