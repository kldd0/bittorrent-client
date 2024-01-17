#ifndef BITTORRENT_CLIENT_BDICT_HPP
#define BITTORRENT_CLIENT_BDICT_HPP

#include <map>
#include <memory>
#include <string>

#include "b_type.hpp"

/**
 * @brief Class dictionary
 *
 * Class representing dictionary in bencoding
 */
class BDict : public BType {
 public:
  /**
   * @brief Construct a new BDict object
   *
   * @param s_pos Index of opening symbol in encoded structure
   * @param e_pos Index of closing symbol in encoded structure
   */
  BDict(int s_pos, int e_pos);

  /**
   * @brief Function for adding element in dict
   *
   * @param key Key of value in dict
   * @param value The object being added
   */
  void add_item(const std::string& key, std::shared_ptr<BType>& value) override;

  /**
   * @brief Get the type object
   *
   * @return const std::string&
   */
  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  /**
   * @brief Get the value dict object
   *
   * @return const std::map<std::string, std::shared_ptr<BType>>&
   */
  [[nodiscard]] const std::map<std::string, std::shared_ptr<BType>>& get_value()
      const {
    return m_dict;
  }

  /**
   * @brief Get the start pos object in byte buffer
   *
   * @return const int&
   */
  const int& get_start_pos() const { return m_start_pos; }
  /**
   * @brief Get the end pos object in byte buffer
   *
   * @return const int&
   */
  const int& get_end_pos() const { return m_end_pos; }

 private:
  int m_start_pos{0};
  int m_end_pos{0};

  std::string m_type{"dict"}; // typename value
  std::map<std::string, std::shared_ptr<BType>> m_dict; // dict value
};

#endif
