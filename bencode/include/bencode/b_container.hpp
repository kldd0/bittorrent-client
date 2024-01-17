#ifndef BITTORRENT_CLIENT_BCONTAINER_HPP
#define BITTORRENT_CLIENT_BCONTAINER_HPP

#include <string>
#include <memory>

#include "b_type.hpp"

/**
 * @brief Container interface
 *
 * Interface class for bencoding containers
 */
class BContainer
{
public:
  /**
   * @brief Destroy the BContainer object
   * 
   */
  virtual ~BContainer() = default;

  /**
   * @brief function for adding elements in BList
   * 
   * @param value
   */
  virtual void add_item(std::unique_ptr<BType>&& value);
  /**
   * @brief function for adding elements in BDict
   * 
   * @param key 
   * @param value 
   */
  virtual void add_item(const std::string& key, std::unique_ptr<BType>&& value);
};

#endif

