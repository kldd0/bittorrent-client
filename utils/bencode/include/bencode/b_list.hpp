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
  const std::string& GetType() const override { return m_type; }

  void AddItem(std::unique_ptr<BValue>& value) override;

  const std::vector<std::unique_ptr<BValue>>& GetValue() { return m_vec; }

private:
  std::string m_type {"list"};
  std::vector<std::unique_ptr<BValue>> m_vec;
};

#endif
