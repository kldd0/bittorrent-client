#ifndef BITTORRENT_CLIENT_BVALUE_HPP
#define BITTORRENT_CLIENT_BVALUE_HPP

#include <string>
#include <memory>

/*
 * Abstract class for different values of bencoding
*/
class BValue
{
public:
  virtual ~BValue() = default;

  [[nodiscard]] virtual const std::string& get_type() const = 0;
  virtual void add_item(std::unique_ptr<BValue>& value) {};
  virtual void add_item(const std::string& key, std::unique_ptr<BValue>& value) {};
  // virtual void add_item(std::string key, std::unique_ptr<BValue> value) {};
};

#endif
