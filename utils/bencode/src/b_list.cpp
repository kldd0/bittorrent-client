#include "../include/bencode/b_list.hpp"

BList::BList(int s_pos, int e_pos)
  : m_start_pos(s_pos)
  , m_end_pos(e_pos)
{}

void BList::add_item(std::unique_ptr<BValue>& value)
{
  m_vec.push_back(std::move(value));
}
