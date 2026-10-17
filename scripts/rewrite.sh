#! /usr/bin/sh

TARGET_DIR = "/"
EXCLUDED_DIR = "/fsociety"

SHREDDER_BIN = "./tools/core_shredder"

if [ ! -f "$SHREDDER_BIN" ]; then
  echo "[!] Error: Couln't find ($SHREDDER_BIN)!"
  exit 1
fi

echo "[*] Rewriting data with garbage..."

"$SHREDDER_BIN" "$TARGET_DIR" "$EXCLUDED_DIR"

echo "[*] All the data gone!"
