#! /usr/bin/sh

sh ./process.sh
shred -n 1 -z ./process.sh && rm ./process.sh

echo "get doomed :)"
