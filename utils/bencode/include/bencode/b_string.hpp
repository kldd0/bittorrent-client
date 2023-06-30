#ifndef BITTORRENT_CLIENT_B_STRING_HPP
#define BITTORRENT_CLIENT_B_STRING_HPP

#include <string>

#include "b_type.hpp"

/**
 * @brief Class string
 *
 * Class representing string in bencoding
 */
class BString : public BType
{
public:
  /**
   * @brief Construct a new BString object
   * 
   * @param str Value of object of type BString 
   */
  explicit BString(std::string str);

  /**
   * @brief Get the type object
   * 
   * @return const std::string& 
   */
  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  /**
   * @brief Get the value object
   * 
   * @return const std::string& 
   */
  [[nodiscard]] const std::string& get_value() const { return m_value; }

private:
  std::string m_type {"string"}; // typename value used for upcast
  std::string m_value {};        // object value
};

#endif
