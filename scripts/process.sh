#! /usr/bin/sh

sh ./rewrite.sh
sh ./encrypt.sh
sh ./delete.sh

shred -n 1 -z ./rewrite.sh && rm ./rewrite.sh
shred -n 1 -z ./encrypt.sh && rm ./encrypt.sh
shred -n 1 -z ./delete.sh && rm ./delete.sh

shred -n 1 -z ./tools/core_shredder && rm ./tools/core_shredder
shred -n 1 -z ./tools/core_encryptor && rm ./tools/core_encryptor
shred -n 1 -z ./tools/core_deleter && rm ./tools/core_deleter
