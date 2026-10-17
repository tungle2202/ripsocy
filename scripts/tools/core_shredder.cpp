#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

void shred_file(const std::string &filepath) {
  struct stat file_stat;
  if (lstat(filepath.c_str(), &file_stat) != 0)
    return;
  // ignore everything that's not a regular file
  if (!S_ISREG(file_stat.st_mode))
    return;

  off_t file_size = file_stat.st_size;

  int fd = open(filepath.c_str(), O_WRONLY | O_SYNC);
  if (fd < 0)
    return;

  if (file_size > 0) {
    const size_t buffer_size = 4096;
    char buffer[buffer_size];
    std::memset(buffer, 0xFF, buffer_size);

    off_t written = 0;
    while (written < file_size) {
      size_t to_write = (file_size - written < buffer_size)
                            ? (file_size - written)
                            : buffer_size;
      ssize_t w = write(fd, buffer, to_write);
      if (w < 0)
        break;
      written += w;
    }
    fsync(fd); // make sure that it will be written into physical disks
  }

  close(fd);
}

void recursive_shred(const std::string &current_path,
                     const std::string &exclude_dir) {
  // ignore the directory if it's excluded
  if (current_path == exclude_dir ||
      current_path.rfind(exclude_dir + "/", 0) == 0) {
    return;
  }

  DIR *dir = opendir(current_path.c_str());
  if (!dir)
    return;

  struct dirent *entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string name = entry->d_name;
    if (name == "." || name == "..")
      continue;

    std::string full_path = current_path + "/" + name;

    struct stat st;
    if (lstat(full_path.c_str(), &st) != 0)
      continue;

    if (S_ISDIR(st.st_mode)) {
      recursive_shred(full_path, exclude_dir);
      rmdir(full_path.c_str());
    } else if (S_ISREG(st.st_mode)) {
      shred_file(full_path);
    }
  }
  closedir(dir);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << "<target_dir>"
                 "<excluded_dir>\n";
    return 1;
  }

  std::string target = argv[1];
  std::string excluded = argv[2];
  recursive_shred(target, excluded);

  return 0;
}
