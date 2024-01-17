#ifndef BITTORRENT_CLIENT_BENCODE_OBJECT_HPP
#define BITTORRENT_CLIENT_BENCODE_OBJECT_HPP

#include <utility>
#include <vector>

#include "b_dict.hpp"
#include "b_type.hpp"

/**
 * @brief Class bencoded object
 *
 * Class representing the entity of the bencoded object,
 * it stores all parsed fields
 */
class BencodeObject {
 public:
  /**
   * @brief Construct a new Bencode Object object
   * 
   * @param buffer Raw bytes of file stream
   */
  BencodeObject(const std::vector<unsigned char>& buffer);

  /**
   * @brief Get the dict object
   * 
   * @return const std::shared_ptr<const BDict> 
   */
  [[nodiscard]] const std::shared_ptr<const BDict> get_dict() {
    return m_dict;
  }
  /**
   * @brief Get the buffer object
   * 
   * @return const std::vector<unsigned char>& 
   */
  [[nodiscard]] const std::vector<unsigned char>& get_buffer() const {
    return m_buffer;
  }

 private:
  /**
   * @brief Main function for parsing byte stream
   * 
   * @param b_object Output structure 
   * @param buffer Raw byte stream
   * @param index Starting index for parsing
   * @param end Ending index for parsing
   */
  void parse(std::shared_ptr<BType>& b_object,
             const std::vector<unsigned char>& buffer, int& index, int& end);

  /**
   * @brief Helper function used in parsing for getting closing character
   * 
   * @param p 
   * @return int 
   */
  int get_index_of_closing_pair(std::pair<int, unsigned char> p);

 private:
  std::shared_ptr<BDict> m_dict; // value of main dict

  std::vector<unsigned char> m_buffer{}; // raw byte buffer
  std::vector<std::pair<int, unsigned char>> m_pairs{}; // pairs of opening and closing indexes
                                                        // of symbols of structures
};

#endif
