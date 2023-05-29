#include "../include/bencode/b_list.hpp"

void BList::add_item(std::unique_ptr<BValue>& value)
{
  m_vec.push_back(std::move(value));
}
