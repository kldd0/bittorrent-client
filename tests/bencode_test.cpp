#include <iostream>
#include <vector>

#include <bencode/b_parser.hpp>

std::vector<std::pair<int, unsigned char>> ParseStream(const std::vector<unsigned char>& buffer)
{
  std::vector<std::pair<int, unsigned char>> pairs;
  int i = 0;
  std::cout << buffer.size() << "\n";
  while(i < buffer.size()) {
    if (std::isdigit(buffer[i])) {
      int strlen = 0;
      while (buffer[i] != ':') {
        strlen = strlen * 10 + (buffer[i] - '0');
        ++i;
      }
      // now buffer[i] = ':'
      ++i;
      std::string str(buffer.begin() + i, buffer.begin() + i + strlen);
      std::cout << str << "\n";
      i += strlen;
    }
    if (buffer[i] == 'i') {
      ++i;
      long long num = 0;
      while (buffer[i] != 'e') {
        num = num * 10 + (buffer[i] - '0');
        ++i;
      }
      std::cout << num << "\n";
      ++i;
    }
    if (buffer[i] == 'd' || buffer[i] == 'l' || buffer[i] == 'e') {
      pairs.push_back(std::make_pair(i, buffer[i]));
      ++i;
    } 
  }
  return pairs;
}

int main() {
  std::string line = "d8:announce33:udp://tracker.opentrackr.org:133710:created by13:mktorrent 1.113:creation datei1682071490e4:infod6:lengthi4081973248e4:name39:manjaro-gnome-22.1.0-230421-linux61.iso12:piece lengthi2097152e6:pieces11:byte_stringee";
  std::vector<unsigned char> buffer;
  for (auto e : line) {
    buffer.push_back(e);
  }

  auto result = ParseStream(buffer);
  for (auto elem : result) {
    std::cout << elem.first << " " << elem.second << "\n";
  }
  return 0;
}
