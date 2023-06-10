#ifndef BITTORRENT_CLIENT_BTYPE_HPP
#define BITTORRENT_CLIENT_BTYPE_HPP

#include <map>
#include <variant>
#include <string>
#include <memory>
#include <utility>
#include <vector>

/**
 * Interface class for different types of bencoding
 */
class BType
{
public:
  virtual ~BType() = default;

  [[nodiscard]] virtual const std::string& get_type() const = 0;
  // [[nodiscard]] virtual const std::shared_ptr<const BType> get_value() const;

  // function for adding elements in BList
  virtual void add_item(std::shared_ptr<BType>& value) {};
  // function for adding elements in BDict
  virtual void add_item(const std::string& key, std::shared_ptr<BType>& value) {};
};

#endif
