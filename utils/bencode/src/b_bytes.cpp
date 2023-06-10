#include "../include/bencode/b_bytes.hpp"

BBytes::BBytes(std::pair<int, int> pair_of_indexes)
  : m_start_pos(pair_of_indexes.first)
  , m_end_pos(pair_of_indexes.second)
{}
