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
  const std::string& GetType() const override { return m_type; }

  void AddItem(std::string key, std::unique_ptr<BValue> value) override;
  void AddItem(std::string key, std::unique_ptr<BValue>& value) override;

  const std::map<std::string, std::unique_ptr<BValue>>& GetValue() { return m_dict; }

private:
  std::string m_type {"dict"};
  std::map<std::string, std::unique_ptr<BValue>> m_dict;
};

#endif
