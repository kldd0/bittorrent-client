#ifndef BITTORRENT_CLIENT_BLIST_HPP
#define BITTORRENT_CLIENT_BLIST_HPP

#include <vector>
#include <memory>
#include <string>

#include "b_value.hpp"

/*
 * Class representing list in bencoding
*/
class BList : public BValue
{
public:
  BList(int s_pos, int e_pos);

  void add_item(std::unique_ptr<BValue>& value) override;

  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  [[nodiscard]] const std::vector<std::unique_ptr<BValue>>& get_value() const { return m_vec; }
  
  const int& get_start_pos() const { return m_start_pos; }
  const int& get_end_pos()   const { return m_end_pos; }

private:
  int m_start_pos {0};
  int m_end_pos {0};

  std::string m_type {"list"};
  std::vector<std::unique_ptr<BValue>> m_vec;
};

#endif
