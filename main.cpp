#include <iostream>
#include <string>
#include <fstream>

#include <boost/asio.hpp>

#include <torrent/torrent_file.hpp>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "You didn't specify the path to the file" << "\n";
    return 0;
  }
  std::string filename(argv[1]);

  std::ifstream input(filename, std::ios::binary);

  auto test = TorrentFile(filename);

  return 0;
}
