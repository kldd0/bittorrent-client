#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <utility>
#include <iterator>
#include <iostream>
#include <algorithm>

#include "../include/bencode/b_types.hpp"
#include "../include/bencode/b_parser.hpp"

[[maybe_unused]]
std::string parse_string(const std::vector<unsigned char>& buffer, int& i)
{
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

[[maybe_unused]]
long long parse_integer(const std::vector<unsigned char>& buffer, int& i)
{
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

std::vector<std::pair<int, unsigned char>> parse_stream(const std::vector<unsigned char>& buffer)
{
  std::vector<std::pair<int, unsigned char>> pairs;
  int i = 0;
  std::cout << buffer.size() << "\n";
  while(i < buffer.size()) {
    if (std::isdigit(buffer[i])) {
      // discarding returned value (parsing)
      std::cout << parse_string(buffer, i) << "\n";
      continue;
    }
    if (buffer[i] == 'i') {
      // discarding returned value (parsing)
      std::cout << parse_integer(buffer, i) << "\n";
      continue;
    }
    if (buffer[i] == 'd' || buffer[i] == 'l' || buffer[i] == 'e') {
      pairs.emplace_back(i, buffer[i]);
      ++i;
    } 
  }
  return pairs;
}

int BencodeParser::get_index_of_closing_pair(std::pair<int, unsigned char> p)
{
  // algo:
  // putting an element for which we need to find a closing pair
  // if we meet 'e' then delete back elem from stack
  // if stack become empty that means we found closing pair
  std::vector<unsigned char> stack;
  // put starting elem
  std::cout << "finding closing pair for: " << p.first << " " << p.second << "\n";
  stack.push_back(p.second);
  int index = -1;
  for (auto it = std::find(m_pairs.begin(), m_pairs.end(), p) + 1; it != m_pairs.end(); ++it) {
    if (it->second == 'e' && (stack.back() != 'e')) {
      stack.pop_back();
      if (stack.empty() || (stack.back() == 'e')) {
        index = it->first;
        std::cout << "found index = " << index << "\n";
        break;
      }
    } else {
      stack.push_back(it->second);
    }
  }
  return index;
}

void BencodeParser::parse(std::unique_ptr<BValue>& b_object, const std::vector<unsigned char>& buffer, int& index, int& end)
{
  if (b_object->get_type() == "dict") {
    while (index != end) {
      std::cout << buffer[index] << "\n";
      std::string key = parse_string(buffer, index);
      std::cout << "key = " << key << "\n";
      if (std::isdigit(buffer[index])) {
        std::unique_ptr<BValue> value = std::make_unique<BString>(parse_string(buffer, index));
        // adding item in dict
        // auto s_val = dynamic_cast<BString>(value);
        // std::cout << "value = " << s_val->get_value() << "\n";
        b_object->add_item(key, value);
        std::cout << "added successfully" << "\n";
      }
      if (buffer[index] == 'i') {
        std::unique_ptr<BValue> value = std::make_unique<BInteger>(parse_integer(buffer, index));
        // adding item in dict
        b_object->add_item(key, value);
        std::cout << "added successfully" << "\n";
      }
      if (buffer[index] == 'd') {
        std::cout << "creating dict" << "\n";
        std::unique_ptr<BValue> value = std::make_unique<BDict>();
        // getting index of ending symbol for this dictionary
        int end_index = get_index_of_closing_pair(std::make_pair(index, buffer[index]));
        std::cout << "end = " << buffer[end_index] << " " << end_index << "\n";
        // as buffer[index] == 'd' => 
        // index needs to be incremented
        ++index;
        // recursively parsing value dict
        std::cout << "recursive parse starts: " << "starting i: " << index << " ending i: " << end_index << "\n";
        parse(value, buffer, index, end_index);
        // adding item in dict
        b_object->add_item(key, value);
        // buffer[index] == 'e' => incrementing
        std::cout << "added successfully" << "\n";
        ++index;
      }
      if (buffer[index] == 'l') {
        std::unique_ptr<BValue> value = std::make_unique<BList>();
        // getting index of ending symbol for this list
        int end_index = get_index_of_closing_pair(std::make_pair(index, buffer[index]));
        std::cout << "end = " << buffer[end_index] << " " << end_index << "\n";
        // as buffer[index] == 'l' => 
        // index needs to be incremented
        ++index;
        // recursively parsing value list
        std::cout << "recursive parse starts: " << "starting i: " << index << " ending i: " << end_index << "\n";
        parse(value, buffer, index, end_index);
        // adding item in dict
        b_object->add_item(key, value);
        // buffer[index] == 'e' => incrementing
        std::cout << "added successfully" << "\n";
        ++index;
      }
    }
  } else if (b_object->get_type() == "list") {
    while (index != end) {
      if (std::isdigit(buffer[index])) {
        std::unique_ptr<BValue> value = std::make_unique<BString>(parse_string(buffer, index));
        // adding item in list
        b_object->add_item(value);
        std::cout << "added successfully" << "\n";
      }
      if (buffer[index] == 'i') {
        std::unique_ptr<BValue> value = std::make_unique<BInteger>(parse_integer(buffer, index));
        // adding item in list
        b_object->add_item(value);
        std::cout << "added successfully" << "\n";
      }
      if (buffer[index] == 'd') {
        std::unique_ptr<BValue> value = std::make_unique<BDict>();
        // getting index of ending symbol for this dictionary
        int end_index = get_index_of_closing_pair(std::make_pair(index, buffer[index]));
        std::cout << "end = " << buffer[end_index] << " " << end_index << "\n";
        // as buffer[index] == 'd' => 
        // index needs to be incremented
        ++index;
        // recursively parsing value dict
        std::cout << "recursive parse starts: " << "starting i: " << index << " ending i: " << end_index << "\n";
        parse(value, buffer, index, end_index);
        // adding item in list
        b_object->add_item(value);
        // buffer[index] == 'e' => incrementing
        std::cout << "added successfully" << "\n";
        ++index;
      }
      if (buffer[index] == 'l') {
        std::unique_ptr<BValue> value = std::make_unique<BList>();
        // getting index of ending symbol for this list
        int end_index = get_index_of_closing_pair(std::make_pair(index, buffer[index]));
        std::cout << "end = " << buffer[end_index] << " " << end_index << "\n";
        // as buffer[index] == 'l' => 
        // index needs to be incremented
        ++index;
        // recursively parsing value list
        std::cout << "recursive parse starts: " << "starting i: " << index << " ending i: " << end_index << "\n";
        parse(value, buffer, index, end_index);
        // adding item in dict
        b_object->add_item(value);
        // buffer[index] == 'e' => incrementing
        std::cout << "added successfully" << "\n";
        ++index;
      }
    }
  }
}

BencodeParser::BencodeParser(std::string &filename)
  : m_filename(filename)
{};

void print_dict(BDict* torrent_ptr, std::string s);

void print_list(BList* t_list);

void BencodeParser::parse_file()
{
  std::ifstream input(m_filename, std::ios::binary);
  std::stringstream sstr;

  // filestream vector
  std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
  
  buffer.erase(buffer.end() - 1);
  // initializing main dictionary
  std::unique_ptr<BValue> torrent_object = std::make_unique<BDict>();

  std::cout << "buff size = " << buffer.size() << "\n";
  std::cout << "last elem = " << *(buffer.end() - 1) << "\n";

  // destructuring in pairs of starting and ending symbols
  m_pairs = parse_stream(buffer);

  std::cout << "----------------------- PAIRS -----------------------" << "\n";
  for (auto elem : m_pairs) {
    std::cout << elem.first << " " << elem.second << "\n";
  }
  std::cout << "m_pairs size = " << m_pairs.size() << "\n";

  // initializing iterators
  // +1 because main dictionary is already created
  // removing first and last symbols ex. (d, e)
  int start = 1;
  int end = buffer.size() - 1;

  // parsing file stream
  std::cout << "starting indexes = " << start << " " << end << "\n";
  parse(torrent_object, buffer, start, end);

  BDict* torrent_ptr = dynamic_cast<BDict*>(torrent_object.get());
  std::cout << "-----------------------------------------" << "\n";
  print_dict(torrent_ptr, "");
}

/*
 * @brief [debug helper funcs] functions for pretty print
 */
void print_list(BList* t_list) {
  std::cout << "[";
  for (auto const& v : t_list->get_value()) {
    if (v->get_type() == "string") {
      std::cout << dynamic_cast<BString*>(v.get())->get_value() << " "; 
    }
    if (v->get_type() == "int") {
      std::cout << dynamic_cast<BInteger*>(v.get())->get_value() << " "; 
    }
    if (v->get_type() == "list") {
      std::cout << "value list: ";
      print_list(dynamic_cast<BList*>(v.get()));
    }
    if (v->get_type() == "dict") {
      std::cout << "value dict:\n";
      print_dict(dynamic_cast<BDict*>(v.get()), "\t\t");
    }
    std::cout << "]\n";
  }
}

void print_dict(BDict* torrent_ptr, std::string s) {
  std::cout << s << "{\n";
  for (auto const& [k, v] : torrent_ptr->get_value()) {
    std::cout << s << "key = " << k << " ";
    if (v->get_type() == "string") {
      std::cout << s << "value = " << dynamic_cast<BString*>(v.get())->get_value() << "\n"; 
    }
    if (v->get_type() == "int") {
      std::cout << s << "value = " << dynamic_cast<BInteger*>(v.get())->get_value() << "\n"; 
    }
    if (v->get_type() == "list") {
      std::cout << s << "values list: ";
      print_list(dynamic_cast<BList*>(v.get()));
    }
    if (v->get_type() == "dict") {
      std::cout << "value dict:\n";
      print_dict(dynamic_cast<BDict*>(v.get()), "\t\t");
    }
  }
  std::cout << "}" << s << "\n";
}

