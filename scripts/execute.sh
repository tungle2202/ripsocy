#! /usr/bin/sh

sh ./process.sh
shred -n 3 -z -u ./process.sh

echo "get doomed :)"
