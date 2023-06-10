#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <utility>
#include <cassert>
#include <iterator>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <filesystem>

#include <format>

#include <boost/compute/detail/sha1.hpp>

#include "../include/bencode/b_types.hpp"
#include "../include/bencode/torrent_file.hpp"

[[maybe_unused]] std::string parse_string(const std::vector<unsigned char>& buffer, int& i) {
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

[[maybe_unused]] long long parse_integer(const std::vector<unsigned char>& buffer, int& i) {
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

[[maybe_unused]] std::pair<int, int> parse_bytes(const std::vector<unsigned char>& buffer, int& i) {
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

std::vector<std::pair<int, unsigned char>> parse_stream(const std::vector<unsigned char>& buffer) {
  std::vector<std::pair<int, unsigned char>> pairs;
  int i = 0;
  while(i < buffer.size()) {
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

void print_stack(const std::vector<unsigned char> v) {
  for (auto e : v) {
    std::cout << e << " ";
  }
  std::cout << "\n";
}

int TorrentFile::get_index_of_closing_pair(std::pair<int, unsigned char> p) {
  // algorithm:
  // putting an element for which we need to find a closing pair
  // if we meet 'e' then delete back elem from stack
  // if stack become empty that means we found closing pair
  std::vector<unsigned char> stack;
  // put starting elem
  stack.push_back(p.second);
  int index = -1;
  for (auto it = std::find(m_pairs.begin(), m_pairs.end(), p) + 1; it != m_pairs.end(); ++it) {
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

void TorrentFile::parse(
    std::shared_ptr<BType>& b_object, const std::vector<unsigned char>& buffer, int& index, int& end) {
  if (b_object->get_type() == "dict") {
    while (index != end) {
      std::string key = parse_string(buffer, index);
      if (std::isdigit(buffer[index])) {
        if (key == "pieces") {
          std::cout << "parsing pieces..." << "\n";
          std::shared_ptr<BType> value = std::make_shared<BBytes>(parse_bytes(buffer, index));
          std::cout << "pieces parsed" << "\n";
          // std::cout << "prev elem: " << buffer[index - 1] << " curr elem: " << buffer[index] << " next elem: " << buffer[index + 1] << "\n";
          // adding item in dict
          b_object->add_item(key, value);
        } else {
          std::shared_ptr<BType> value = std::make_shared<BString>(parse_string(buffer, index));
          // adding item in dict
          b_object->add_item(key, value);
        }
      }
      if (buffer[index] == 'i') {
        std::shared_ptr<BType> value = std::make_shared<BInteger>(parse_integer(buffer, index));
        // adding item in dict
        b_object->add_item(key, value);
      }
      if (buffer[index] == 'd') {
        // getting index of ending symbol for this dictionary
        int end_index = get_index_of_closing_pair(std::make_pair(index, buffer[index]));
        std::cout << "found end index for dict: " << key << " start index: " << index << " elem: " << buffer[index] << " end_index: " << end_index << " elem: " << buffer[end_index] << "\n";  
        std::shared_ptr<BType> value = std::make_shared<BDict>(index, end_index);
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
        int end_index = get_index_of_closing_pair(std::make_pair(index, buffer[index]));
        std::shared_ptr<BType> value = std::make_shared<BList>(index, end_index);
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
        std::shared_ptr<BType> value = std::make_shared<BString>(parse_string(buffer, index));
        // adding item in list
        b_object->add_item(value);
      }
      if (buffer[index] == 'i') {
        std::shared_ptr<BType> value = std::make_shared<BInteger>(parse_integer(buffer, index));
        // adding item in list
        b_object->add_item(value);
      }
      if (buffer[index] == 'd') {
        // getting index of ending symbol for this dictionary
        int end_index = get_index_of_closing_pair(std::make_pair(index, buffer[index]));
        std::shared_ptr<BType> value = std::make_shared<BDict>(index, end_index);
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
        int end_index = get_index_of_closing_pair(std::make_pair(index, buffer[index]));
        std::shared_ptr<BType> value = std::make_shared<BList>(index, end_index);
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

TorrentFile::TorrentFile(std::string &filename)
  : m_filename(filename)
{
  parse_file();
};

void print_dict(const std::shared_ptr<BDict>& t_dict, int nested_index);

void print_list(const std::shared_ptr<BList>& t_list, int nested_index);

void TorrentFile::parse_file() {
  std::ifstream input(m_filename, std::ios::binary);
  std::stringstream sstr;

  // filestream vector
  m_buffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(input), {});
  
  // deleting the last element because it's a new line char
  if (*(m_buffer.end() - 1) == ' ') {
    m_buffer.erase(m_buffer.end() - 1);
  }

  // initializing starting and last pos of symbols
  int start = 0;
  int end = m_buffer.size() - 1;

  // initializing main dictionary
  std::shared_ptr<BType> torrent_object = std::make_shared<BDict>(start, end);

  // destructuring in pairs of starting and ending symbols
  m_pairs = parse_stream(m_buffer);

  std::cout << "---- pairs ----" << "\n";
  for (auto e : m_pairs) {
    std::cout << "start: " << e.first << " elem: " << e.second << "\n";
  }
  std::cout << "---------------" << "\n";

  // +1 because main dictionary is already created
  // excluding starting symbols of container ex. 'd'
  ++start;

  // parsing file stream
  parse(torrent_object, m_buffer, start, end);

  auto torrent_ptr = std::dynamic_pointer_cast<BDict>(torrent_object);

  m_data = torrent_ptr;
  print_dict(m_data, 0);

  std::cout << get_info_hash() << "\n";
  create_empty_files();

  std::system("tree download");
}

std::string TorrentFile::get_info_hash() {
  // getting info dict object
  // it's needed for creating a sha1 hash from raw bytes
  auto info_dict = std::dynamic_pointer_cast<BDict>(m_data->get_value().find("info")->second);
  int start = info_dict->get_start_pos();
  int end = info_dict->get_end_pos();

  boost::uuids::detail::sha1 hash;
  boost::uuids::detail::sha1::digest_type digest;

  std::cout << "end: " << m_buffer[end] << " prev: " << m_buffer[end - 1] - '0' << "\n";

  std::vector<unsigned char> info_dict_bytes = std::vector<unsigned char>(m_buffer.begin() + start, m_buffer.begin() + end + 1);
  // auto info_dict_bytes = hash_test_input();
  std::cout << "buff size: " << m_buffer.size() << "\n";
  std::cout << "first elem: " << info_dict_bytes[0]  << " index: " << start << "\n";
  std::cout << "last elem: " << *(info_dict_bytes.end() - 1) << " index: " << end << "\n";

  hash.process_bytes(info_dict_bytes.data(), info_dict_bytes.size());
  hash.get_digest(digest);

  std::ostringstream oss;
  for (int i = 0; i < 5; i++) {
    oss << std::hex << std::setw(8) << std::setfill('0') << digest[i];
  }

  std::string hash_str = oss.str();
  return hash_str; 
}

void fill_file_with_zero_bytes(std::string filename, long long length, std::string path = "") {
  namespace fs = std::filesystem;
  if (path != "") {
    fs::create_directories(path);
  }
  fs::path file_path = path;
  file_path /= filename;
  std::cout << "curr path: " << path + filename << "\n";
  std::ofstream file {file_path, std::ios::binary};
  assert(file.is_open());
  file.seekp(length - 1, std::ios::beg);
  char byte = 0;
  file.write(&byte, 1);
  file.close();
}

void TorrentFile::create_empty_files() {
  namespace fs = std::filesystem;
  try {
    fs::current_path("download");
  } catch (const std::exception& err) {
    fs::create_directory("download");
    fs::current_path("download");
  }
  // getting info dict object
  std::map<std::string, std::shared_ptr<BType>> info_dict = std::dynamic_pointer_cast<BDict>(m_data->get_value().find("info")->second)->get_value();
  auto files_value_it = info_dict.find("files"); 
  if (files_value_it != info_dict.end()) {
    // @ this is a branch of multi-file torrent processing
    // @ "files" structure
    // files: list[ dict{ "length": num_bytes, "path": list[ "folder1", ... "filename"] }, ... ]
    std::vector<std::shared_ptr<BType>> files_list = std::dynamic_pointer_cast<BList>(files_value_it->second)->get_value();
    for (auto const& file_dict : files_list) {
      auto path_list = std::dynamic_pointer_cast<BList>(std::dynamic_pointer_cast<BDict>(file_dict)->get_value().find("path")->second)->get_value();
      long long length = std::dynamic_pointer_cast<BInteger>(std::dynamic_pointer_cast<BDict>(file_dict)->get_value().find("length")->second)->get_value();
      std::string dir_path {};
      for (auto it = path_list.begin(); it != path_list.end() - 1; ++it) {
        dir_path += std::dynamic_pointer_cast<BString>(*it)->get_value();
      }
      std::string filename = std::dynamic_pointer_cast<BString>(*(path_list.end() - 1))->get_value();
      fill_file_with_zero_bytes(filename, length, dir_path);
    }
  } else {
    long long length = std::dynamic_pointer_cast<BInteger>(info_dict.find("length")->second)->get_value();
    std::string filename = std::dynamic_pointer_cast<BString>(info_dict.find("name")->second)->get_value();
    fill_file_with_zero_bytes(filename, length);
  }
}

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
      std::cout << s << std::dynamic_pointer_cast<BString>(v)->get_value() << ","; 
    }
    if (v->get_type() == "int") {
      std::cout << s << std::dynamic_pointer_cast<BInteger>(v)->get_value() << ","; 
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
  std::cout << make_tabs(nested_index - 1) << nested_index << "{" << "\n";
  for (auto const& [k, v] : t_dict->get_value()) {
    std::cout << s << k << ": ";
    if (k == "pieces") {
      std::cout << "OMITTED" << "\n";
      std::cout << s << "}" << nested_index << "\n";
      return;
    }
    if (v->get_type() == "string") {
      std::cout << s << std::dynamic_pointer_cast<BString>(v)->get_value() << "\n"; 
    }
    if (v->get_type() == "int") {
      std::cout << s << std::dynamic_pointer_cast<BInteger>(v)->get_value() << "\n"; 
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
