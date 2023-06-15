#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/compute/detail/sha1.hpp>

#include "../include/torrent/torrent_file.hpp"

TorrentFile::TorrentFile(const std::string& filename) : m_filename(filename) {
  std::ifstream input(m_filename, std::ios::binary);
  std::stringstream sstr;

  // filestream vector
  auto buffer =
      std::vector<unsigned char>(std::istreambuf_iterator<char>(input), {});

  m_data = std::make_shared<BencodeObject>(buffer);

  std::cout << get_info_hash() << "\n";

  create_empty_files();
}

std::string TorrentFile::get_info_hash() {
  // dict of b_object
  auto dict = m_data->get_value();

  // getting info dict object
  // it's needed for creating a sha1 hash from raw bytes
  auto info_dict =
      std::dynamic_pointer_cast<BDict>(dict->get_value().find("info")->second);

  int start = info_dict->get_start_pos();
  int end = info_dict->get_end_pos();

  boost::uuids::detail::sha1 hash;
  boost::uuids::detail::sha1::digest_type digest;

  auto buffer = m_data->get_buffer();

  std::vector<unsigned char> info_dict_bytes = std::vector<unsigned char>(
      buffer.begin() + start, buffer.begin() + end + 1);

  // processing bytes into hash bytes and returning it
  hash.process_bytes(info_dict_bytes.data(), info_dict_bytes.size());
  hash.get_digest(digest);

  std::ostringstream oss;
  for (int i = 0; i < 5; i++) {
    oss << std::hex << std::setw(8) << std::setfill('0') << digest[i];
  }

  std::string hash_str = oss.str();
  return hash_str;
}

void fill_file_with_zero_bytes(std::string filename, long long length,
                               std::string path = "") {
  namespace fs = std::filesystem;
  if (path != "") {
    fs::create_directories(path);
  }
  // making correct path of file
  fs::path file_path = path;
  file_path /= filename;

  std::cout << "curr path: " << path + filename << "\n";

  // open file for reading in binary mode
  std::ofstream file{file_path, std::ios::binary};

  // check if file is open
  if (!file.is_open()) {
    throw std::runtime_error("File not found: " + file_path.string());
  }
  // moving pointer to the end of file
  file.seekp(length - 1, std::ios::beg);

  // writing zero bytes to file
  char byte = 0;
  file.write(&byte, 1);
  file.close();
}

void TorrentFile::create_empty_files() {
  namespace fs = std::filesystem;
  try {
    fs::current_path("download");
  } catch (const std::exception& err) {
    fs::create_directory("download");
    fs::current_path("download");
  }
  // dict of b_object
  auto dict = m_data->get_value();
  // getting info dict object
  std::map<std::string, std::shared_ptr<BType>> info_dict =
      std::dynamic_pointer_cast<BDict>(dict->get_value().find("info")->second)
          ->get_value();
  // getting iterator of files dict value if it exists
  auto files_value_it = info_dict.find("files");
  if (files_value_it != info_dict.end()) {
    // @ this is a branch of multi-file torrent processing
    // @ "files" structure
    // files: l[ d{"length": num_bytes, "path": l[ "dir1", ... "fname"] }, ... ]
    std::vector<std::shared_ptr<BType>> files_list =
        std::dynamic_pointer_cast<BList>(files_value_it->second)->get_value();

    for (auto const& file_dict : files_list) {
      // list of file paths
      auto path_list = std::dynamic_pointer_cast<BList>(
                           std::dynamic_pointer_cast<BDict>(file_dict)
                               ->get_value()
                               .find("path")
                               ->second)
                           ->get_value();

      // length of file (in bytes)
      long long length = std::dynamic_pointer_cast<BInteger>(
                             std::dynamic_pointer_cast<BDict>(file_dict)
                                 ->get_value()
                                 .find("length")
                                 ->second)
                             ->get_value();

      std::string dir_path{};
      for (auto it = path_list.begin(); it != path_list.end() - 1; ++it) {
        dir_path += std::dynamic_pointer_cast<BString>(*it)->get_value();
      }

      std::string filename =
          std::dynamic_pointer_cast<BString>(*(path_list.end() - 1))
              ->get_value();
      // creating file
      fill_file_with_zero_bytes(filename, length, dir_path);
    }
  } else {
    long long length =
        std::dynamic_pointer_cast<BInteger>(info_dict.find("length")->second)
            ->get_value();

    std::string filename =
        std::dynamic_pointer_cast<BString>(info_dict.find("name")->second)
            ->get_value();
    // creating file
    fill_file_with_zero_bytes(filename, length);
  }
}
