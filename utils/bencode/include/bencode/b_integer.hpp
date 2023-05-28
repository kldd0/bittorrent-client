#ifndef BITTORRENT_CLIENT_B_INTEGER_HPP
#define BITTORRENT_CLIENT_B_INTEGER_HPP

#include "b_value.hpp"

/*
 * Class representing string in bencoding
 * type is long long because values can be huge
*/
class BInteger : public BValue
{
public:
  explicit BInteger(long long num);

  const std::string& GetType() const override { return m_type; }
  long long GetValue() const { return m_value; }

private:
  std::string m_type {"int"};
  long long m_value {0};
};

#endif
