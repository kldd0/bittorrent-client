#include "../include/bencode/b_dict.hpp"

void BDict::AddItem(std::string key, std::unique_ptr<BValue>& value)
{
  m_dict.emplace(key, std::move(value));
}

void BDict::AddItem(std::string key, std::unique_ptr<BValue> value)
{
  m_dict.emplace(key, std::move(value));
}
