#! /usr/bin/sh

mkdir /fsociety
cp -r ./scripts/* /fsociety
cd /fsociety
sh ./rewrite.sh
sh ./encrypt.sh
sh ./delete.sh
sh ./clean.sh
