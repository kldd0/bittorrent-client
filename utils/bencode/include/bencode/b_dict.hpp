#ifndef BITTORRENT_CLIENT_BDICT_HPP
#define BITTORRENT_CLIENT_BDICT_HPP

#include <map>
#include <memory>
#include <string>

#include "b_type.hpp"

/**
 * Class representing dictionary in bencoding
 */
class BDict : public BType {
 public:
  BDict(int s_pos, int e_pos);

  void add_item(const std::string& key, std::shared_ptr<BType>& value) override;

  [[nodiscard]] const std::string& get_type() const override { return m_type; }
  [[nodiscard]] const std::map<std::string, std::shared_ptr<BType>>& get_value()
      const {
    return m_dict;
  }

  const int& get_start_pos() const { return m_start_pos; }
  const int& get_end_pos() const { return m_end_pos; }

 private:
  int m_start_pos{0};
  int m_end_pos{0};

  std::string m_type{"dict"};
  std::map<std::string, std::shared_ptr<BType>> m_dict;
};

#endif
