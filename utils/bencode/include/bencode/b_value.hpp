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
  virtual ~BValue() {};

  virtual const std::string& GetType() const = 0;
  virtual void AddItem(std::unique_ptr<BValue>& value) {};
  virtual void AddItem(std::string key, std::unique_ptr<BValue> value) {};
  virtual void AddItem(std::string key, std::unique_ptr<BValue>& value) {};
};

#endif
