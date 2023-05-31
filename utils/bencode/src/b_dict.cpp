#include "../include/bencode/b_dict.hpp"

BDict::BDict(int s_pos, int e_pos)
  : m_start_pos(s_pos)
  , m_end_pos(e_pos)
{}

void BDict::add_item(const std::string& key, std::unique_ptr<BValue>& value)
{
  m_dict.emplace(key, std::move(value));
}

// void BDict::add_item(std::string key, std::unique_ptr<BValue> value)
// {
//   m_dict.emplace(key, std::move(value));
// }
