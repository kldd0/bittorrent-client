#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "../include/bencode/b_types.hpp"

[[maybe_unused]] std::string parse_string(
    const std::vector<unsigned char>& buffer, int& i) {
  int strlen = 0;
  while (buffer[i] != ':') {
    strlen = strlen * 10 + (buffer[i] - '0');
    ++i;
  }
  // now buffer[i] = ':'
  ++i;
  std::string str(buffer.begin() + i, buffer.begin() + i + strlen);
  i += strlen;
  return str;
}

[[maybe_unused]] long long parse_integer(
    const std::vector<unsigned char>& buffer, int& i) {
  // beginning with buffer[i] = 'i'
  ++i;
  long long num = 0;
  while (buffer[i] != 'e') {
    num = num * 10 + (buffer[i] - '0');
    ++i;
  }
  // (buffer[i] = 'e') incrementing i to the next char
  ++i;
  return num;
}

[[maybe_unused]] std::pair<int, int> parse_bytes(
    const std::vector<unsigned char>& buffer, int& i) {
  int seqlen = 0;
  while (buffer[i] != ':') {
    seqlen = seqlen * 10 + (buffer[i] - '0');
    ++i;
  }
  // now buffer[i] = ':'
  ++i;
  int start = i;
  int end = start + seqlen - 1;
  i += seqlen;
  return std::make_pair(start, end);
}

std::vector<std::pair<int, unsigned char>> parse_stream(
    const std::vector<unsigned char>& buffer) {
  std::vector<std::pair<int, unsigned char>> pairs;
  int i = 0;
  while (i < buffer.size()) {
    if (std::isdigit(buffer[i])) {
      // discarding returned value (parsing)
      parse_bytes(buffer, i);
    }
    if (buffer[i] == 'i') {
      // discarding returned value (parsing)
      parse_integer(buffer, i);
    }
    if (buffer[i] == 'd' || buffer[i] == 'l' || buffer[i] == 'e') {
      pairs.emplace_back(i, buffer[i]);
      ++i;
    }
  }
  return pairs;
}

int BencodeObject::get_index_of_closing_pair(std::pair<int, unsigned char> p) {
  // algorithm:
  // putting an element for which we need to find a closing pair
  // if we meet 'e' then delete back elem from stack
  // if stack become empty that means we found closing pair
  std::vector<unsigned char> stack;
  // put starting elem
  stack.push_back(p.second);
  int index = -1;
  for (auto it = std::find(m_pairs.begin(), m_pairs.end(), p) + 1;
       it != m_pairs.end(); ++it) {
    if (it->second == 'e' && (stack.back() != 'e')) {
      stack.pop_back();
      if (stack.empty() || (stack.back() == 'e')) {
        index = it->first;
        return index;
      }
    } else {
      stack.push_back(it->second);
    }
  }
  return index;
}

void BencodeObject::parse(std::shared_ptr<BType>& b_object,
                        const std::vector<unsigned char>& buffer, int& index,
                        int& end) {
  if (b_object->get_type() == "dict") {
    while (index != end) {
      std::string key = parse_string(buffer, index);
      if (std::isdigit(buffer[index])) {
        if (key == "pieces") {
          std::cout << "parsing pieces..."
                    << "\n";
          std::shared_ptr<BType> value =
              std::make_shared<BBytes>(parse_bytes(buffer, index));
          std::cout << "pieces parsed"
                    << "\n";
          // adding item in dict
          b_object->add_item(key, value);
        } else {
          std::shared_ptr<BType> value =
              std::make_shared<BString>(parse_string(buffer, index));
          // adding item in dict
          b_object->add_item(key, value);
        }
      }
      if (buffer[index] == 'i') {
        std::shared_ptr<BType> value =
            std::make_shared<BInteger>(parse_integer(buffer, index));
        // adding item in dict
        b_object->add_item(key, value);
      }
      if (buffer[index] == 'd') {
        // getting index of ending symbol for this dictionary
        int end_index =
            get_index_of_closing_pair(std::make_pair(index, buffer[index]));

        std::cout << "found end index for dict: " << key
                  << " start index: " << index << " elem: " << buffer[index]
                  << " end_index: " << end_index
                  << " elem: " << buffer[end_index] << "\n";

        std::shared_ptr<BType> value =
            std::make_shared<BDict>(index, end_index);
        // as buffer[index] == 'd' =>
        // index needs to be incremented
        ++index;
        // recursively parsing value dict
        parse(value, buffer, index, end_index);
        // adding item in dict
        b_object->add_item(key, value);
        // buffer[index] == 'e' => incrementing
        ++index;
      }
      if (buffer[index] == 'l') {
        // getting index of ending symbol for this list
        int end_index =
            get_index_of_closing_pair(std::make_pair(index, buffer[index]));

        std::shared_ptr<BType> value =
            std::make_shared<BList>(index, end_index);
        // as buffer[index] == 'l' =>
        // index needs to be incremented
        ++index;
        // recursively parsing value list
        parse(value, buffer, index, end_index);
        // adding item in dict
        b_object->add_item(key, value);
        // buffer[index] == 'e' => incrementing
        ++index;
      }
    }
  } else if (b_object->get_type() == "list") {
    while (index != end) {
      if (std::isdigit(buffer[index])) {
        std::shared_ptr<BType> value =
            std::make_shared<BString>(parse_string(buffer, index));
        // adding item in list
        b_object->add_item(value);
      }
      if (buffer[index] == 'i') {
        std::shared_ptr<BType> value =
            std::make_shared<BInteger>(parse_integer(buffer, index));
        // adding item in list
        b_object->add_item(value);
      }
      if (buffer[index] == 'd') {
        // getting index of ending symbol for this dictionary
        int end_index =
            get_index_of_closing_pair(std::make_pair(index, buffer[index]));

        std::shared_ptr<BType> value =
            std::make_shared<BDict>(index, end_index);
        // as buffer[index] == 'd' =>
        // index needs to be incremented
        ++index;
        // recursively parsing value dict
        parse(value, buffer, index, end_index);
        // adding item in list
        b_object->add_item(value);
        // buffer[index] == 'e' => incrementing
        ++index;
      }
      if (buffer[index] == 'l') {
        // getting index of ending symbol for this list
        int end_index =
            get_index_of_closing_pair(std::make_pair(index, buffer[index]));

        std::shared_ptr<BType> value =
            std::make_shared<BList>(index, end_index);
        // as buffer[index] == 'l' =>
        // index needs to be incremented
        ++index;
        // recursively parsing value list
        parse(value, buffer, index, end_index);
        // adding item in dict
        b_object->add_item(value);
        // buffer[index] == 'e' => incrementing
        ++index;
      }
    }
  }
}


void print_dict(const std::shared_ptr<BDict>& t_dict, int nested_index);

void print_list(const std::shared_ptr<BList>& t_list, int nested_index);


BencodeObject::BencodeObject(const std::vector<unsigned char>& buffer)
  : m_buffer(buffer) {
  // clear an ending new line character
  if (*(m_buffer.end() - 1) == ' ') {
    m_buffer.erase(m_buffer.end() - 1);
  }

  // initializing starting and last pos of symbols
  int start = 0;
  int end = m_buffer.size() - 1;

  // initializing main dictionary
  std::shared_ptr<BType> main_dict = std::make_shared<BDict>(start, end);

  // destructuring in pairs of starting and ending symbols
  m_pairs = parse_stream(m_buffer);

  std::cout << "---- pairs ----"
            << "\n";
  for (auto e : m_pairs) {
    std::cout << "start: " << e.first << " elem: " << e.second << "\n";
  }
  std::cout << "---------------"
            << "\n";

  // +1 because main dictionary is already created
  // excluding starting symbols of container ex. 'd'
  ++start;

  // parsing file stream
  parse(main_dict, m_buffer, start, end);

  m_dict = std::dynamic_pointer_cast<BDict>(main_dict);

  print_dict(m_dict, 0);
};


/*
 * @brief [debug helper funcs] functions for pretty print
 */
std::string make_tabs(int n) {
  std::string out;
  while (n > 0) {
    out += "\t";
    --n;
  }
  return out;
}

void print_list(const std::shared_ptr<BList>& t_list, int nested_index) {
  std::string s = make_tabs(nested_index);
  std::cout << "[";
  for (auto const& v : t_list->get_value()) {
    if (v->get_type() == "string") {
      std::cout << s << std::dynamic_pointer_cast<BString>(v)->get_value()
                << ",";
    }
    if (v->get_type() == "int") {
      std::cout << s << std::dynamic_pointer_cast<BInteger>(v)->get_value()
                << ",";
    }
    if (v->get_type() == "list") {
      print_list(std::dynamic_pointer_cast<BList>(v), nested_index + 1);
    }
    if (v->get_type() == "dict") {
      print_dict(std::dynamic_pointer_cast<BDict>(v), nested_index + 1);
    }
    std::cout << s << "]";
  }
}

void print_dict(const std::shared_ptr<BDict>& t_dict, int nested_index) {
  std::string s = make_tabs(nested_index + 1);
  std::cout << make_tabs(nested_index - 1) << nested_index << "{"
            << "\n";
  for (auto const& [k, v] : t_dict->get_value()) {
    std::cout << s << k << ": ";
    if (k == "pieces") {
      std::cout << "OMITTED"
                << "\n";
      std::cout << s << "}" << nested_index << "\n";
      return;
    }
    if (v->get_type() == "string") {
      std::cout << s << std::dynamic_pointer_cast<BString>(v)->get_value()
                << "\n";
    }
    if (v->get_type() == "int") {
      std::cout << s << std::dynamic_pointer_cast<BInteger>(v)->get_value()
                << "\n";
    }
    if (v->get_type() == "list") {
      print_list(std::dynamic_pointer_cast<BList>(v), nested_index + 1);
    }
    if (v->get_type() == "dict") {
      print_dict(std::dynamic_pointer_cast<BDict>(v), nested_index + 1);
    }
  }
  std::cout << make_tabs(nested_index - 1) << "}" << nested_index << "\n";
}
