#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <utility>

#include <boost/asio.hpp>

#include <torrent/tracker.hpp>
#include <torrent/torrent_file.hpp>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "You didn't specify the path to the file" << "\n";
    return 0;
  }
  std::string filename(argv[1]);

  std::ifstream input(filename, std::ios::binary);

  auto torrent = std::make_unique<TorrentFile>(filename);
  auto tracker = Tracker(std::move(torrent));

  return 0;
}
