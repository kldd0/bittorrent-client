#ifndef BITTORRENT_CLIENT_BTYPE_HPP
#define BITTORRENT_CLIENT_BTYPE_HPP

#include <map>
#include <variant>
#include <string>
#include <memory>
#include <utility>
#include <vector>

/**
 * @brief Bencoding type interface
 *
 * Interface class for different types of bencoding
 */
class BType
{
public:
  /**
   * @brief Destroy the BType object
   * 
   */
  virtual ~BType() = default;

  /**
   * @brief Get the type object
   * 
   * @return const std::string& 
   */
  [[nodiscard]] virtual const std::string& get_type() const = 0;

  /**
   * @brief Function for adding elements in BList
   * 
   * @param value 
   */
  virtual void add_item(std::shared_ptr<BType>& value) {};
  /**
   * @brief Function for adding elements in BDict
   * 
   * @param key
   * @param value 
   */
  virtual void add_item(const std::string& key, std::shared_ptr<BType>& value) {};
};

#endif
