#ifndef BITTORRENT_CLIENT_B_STRING_HPP
#define BITTORRENT_CLIENT_B_STRING_HPP

#include <string>

#include "b_value.hpp"

/*
 * Class representing string in bencoding
*/
class BString : public BValue
{
public:
  explicit BString(const std::string& str);

  const std::string& GetType() const override { return m_type; }
  const std::string& GetValue() const { return m_value; }

private:
  std::string m_type {"string"};
  std::string m_value {};
};

#endif
