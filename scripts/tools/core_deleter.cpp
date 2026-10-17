#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

void remove_inode(const std::string &filepath) {
  struct stat st;
  if (lstat(filepath.c_str(), &st) != 0)
    return;

  if (S_ISREG(st.st_mode)) {
    if (unlink(filepath.c_str()) != 0) {
      // ignore, it's all done anyway
    }
  }
}

void recursive_purge(const std::string &current_path,
                     const std::string &exclude_dir) {
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
      recursive_purge(full_path, exclude_dir);

      rmdir(full_path.c_str());
    } else if (S_ISREG(st.st_mode)) {
      remove_inode(full_path);
    }
  }
  closedir(dir);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <target_dir> <exclude_dir>\n";
    return 1;
  }

  std::string target = argv[1];
  std::string exclude = argv[2];

  recursive_purge(target, exclude);

  return 0;
}
