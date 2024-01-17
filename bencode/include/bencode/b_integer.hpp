#ifndef BITTORRENT_CLIENT_B_INTEGER_HPP
#define BITTORRENT_CLIENT_B_INTEGER_HPP

#include "b_type.hpp"

/**
 * @brief Class integer
 *
 * Class representing numerical type in bencoding
 * type is long long because values can be huge
 */
class BInteger : public BType
{
public:
  /**
   * @brief Construct a new BInteger object
   * 
   * @param num Value of integer object
   */
  explicit BInteger(long long num);

  /**
   * @brief Get the type object
   * 
   * @return const std::string& 
   */
  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  /**
   * @brief Get the value object
   * 
   * @return long long 
   */
  [[nodiscard]] long long get_value() const { return m_value; }

private:
  std::string m_type {"int"}; // typename value
  long long m_value {0};
};

#endif
