#! /usr/bin/sh

sh ./rewrite.sh
sh ./encrypt.sh
sh ./delete.sh

shred -n 3 -u -z ./rewite.sh
shred -n 3 -u -z ./encrypt.sh
shred -n 3 -u -z ./delete.sh

shred -n 3 -u -z ./tools/core_shredder
shred -n 3 -u -z ./tools/core_encryptor
shred -n 3 -u -z ./tools/core_deleter
