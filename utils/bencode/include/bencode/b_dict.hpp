#ifndef BITTORRENT_CLIENT_BDICT_HPP
#define BITTORRENT_CLIENT_BDICT_HPP

#include <map>
#include <memory>
#include <string>

#include "b_value.hpp"

/*
 * Class representing dictionary in bencoding
*/
class BDict : public BValue
{
public:
  void add_item(const std::string& key, std::unique_ptr<BValue>& value) override;
  // void add_item(std::string key, std::unique_ptr<BValue> value) override;
  
  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  [[nodiscard]] const std::map<std::string, std::unique_ptr<BValue>>& get_value() { return m_dict; }

private:
  std::string m_type {"dict"};
  std::map<std::string, std::unique_ptr<BValue>> m_dict;
};

#endif
