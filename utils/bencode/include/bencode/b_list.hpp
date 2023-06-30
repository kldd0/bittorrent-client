#ifndef BITTORRENT_CLIENT_BLIST_HPP
#define BITTORRENT_CLIENT_BLIST_HPP

#include <vector>
#include <memory>
#include <string>

#include "b_type.hpp"

/**
 * @brief Class list
 *
 * Class representing list in bencoding
 */
class BList : public BType
{
public:
  /**
   * @brief Construct a new BList object
   * 
   * @param s_pos Index of opening symbol in encoded structure
   * @param e_pos Index of closing symbol in encoded structure
   */
  BList(int s_pos, int e_pos);

  /**
   * @brief Function for adding element in list 
   * 
   * @param value The object being added
   */
  void add_item(std::shared_ptr<BType>& value) override;

  /**
   * @brief Get the type object
   * 
   * @return const std::string& 
   */
  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  /**
   * @brief Get the value object
   * 
   * @return const std::vector<std::shared_ptr<BType>>& 
   */
  [[nodiscard]] const std::vector<std::shared_ptr<BType>>& get_value() const { return m_vec; }

  /**
   * @brief Get the index of opening symbol of dict in encoded structure
   * 
   * @return const int& 
   */
  const int& get_start_pos() const { return m_start_pos; }
  /**
   * @brief Get the index of closing symbol of dict in encoded structure
   * 
   * @return const int& 
   */
  const int& get_end_pos()   const { return m_end_pos; }

private:
  int m_start_pos {0};
  int m_end_pos {0};

  std::string m_type {"list"}; // typename value
  std::vector<std::shared_ptr<BType>> m_vec; // list value of objects
};

#endif
