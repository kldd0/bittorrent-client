#include <iostream>
#include <vector>


int main() {
  std::string line = "d8:announce33:udp://tracker.opentrackr.org:133710:created by13:mktorrent 1.113:creation datei1682071490e4:infod6:lengthi4081973248e4:name39:manjaro-gnome-22.1.0-230421-linux61.iso12:piece lengthi2097152e6:pieces11:byte_stringee";
  std::vector<unsigned char> buffer;
  for (auto e : line) {
    buffer.push_back(e);
  }

  // for (auto elem : result) {
  //   std::cout << elem.first << " " << elem.second << "\n";
  // }
  return 0;
}
