#ifndef BITTORRENT_CLIENT_BENCODE_OBJECT_HPP
#define BITTORRENT_CLIENT_BENCODE_OBJECT_HPP

#include <utility>
#include <vector>

#include "b_dict.hpp"
#include "b_type.hpp"

class BencodeObject {
 public:
  BencodeObject(const std::vector<unsigned char>& buffer);

  [[nodiscard]] const std::shared_ptr<const BDict> get_dict() {
    return m_dict;
  }
  [[nodiscard]] const std::vector<unsigned char>& get_buffer() const {
    return m_buffer;
  }

 private:
  // main function for parsing byte stream
  void parse(std::shared_ptr<BType>& b_object,
             const std::vector<unsigned char>& buffer, int& index, int& end);

  // helper function used in parsing for getting closing character
  int get_index_of_closing_pair(std::pair<int, unsigned char> p);

 private:
  std::shared_ptr<BDict> m_dict;

  std::vector<unsigned char> m_buffer{};
  std::vector<std::pair<int, unsigned char>> m_pairs{};
};

#endif
