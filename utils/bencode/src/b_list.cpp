#include "../include/bencode/b_list.hpp"

void BList::AddItem(std::unique_ptr<BValue>& value)
{
  m_vec.push_back(std::move(value));
}
