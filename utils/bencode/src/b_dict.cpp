#include "../include/bencode/b_dict.hpp"

void BDict::add_item(const std::string& key, std::unique_ptr<BValue>& value)
{
  m_dict.emplace(key, std::move(value));
}

// void BDict::add_item(std::string key, std::unique_ptr<BValue> value)
// {
//   m_dict.emplace(key, std::move(value));
// }
