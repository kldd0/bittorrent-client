#include "../include/bencode/b_string.hpp"

BString::BString(const std::string& str)
  : m_value(str)
{}
