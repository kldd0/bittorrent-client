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
  void add_item(std::unique_ptr<BValue>& value) override;

  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  [[nodiscard]] const std::vector<std::unique_ptr<BValue>>& get_value() { return m_vec; }

private:
  std::string m_type {"list"};
  std::vector<std::unique_ptr<BValue>> m_vec;
};

#endif
