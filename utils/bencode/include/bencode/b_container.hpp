#ifndef BITTORRENT_CLIENT_BCONTAINER_HPP
#define BITTORRENT_CLIENT_BCONTAINER_HPP

#include <string>
#include <memory>

#include "b_type.hpp"

/**
 * Interface class for bencoding containers
 */
class BContainer
{
public:
  virtual ~BContainer() = default;

  // function for adding elements in BList
  virtual void add_item(std::unique_ptr<BType>&& value);
  // function for adding elements in BDict
  virtual void add_item(const std::string& key, std::unique_ptr<BType>&& value);
};

#endif

