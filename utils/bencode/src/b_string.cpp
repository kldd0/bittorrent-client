#include <utility>

#include "../include/bencode/b_string.hpp"

BString::BString(std::string str)
  : m_value(std::move(str))
{}
