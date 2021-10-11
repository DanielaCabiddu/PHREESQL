#!/bin/bash

mkdir -p build
cd build

rm -rf db.db

cmake ../phreesqltool
make

./phreesql -d ../db.db -i ../data/SHORT_SHORT_DB/IN -o ../data/SHORT_SHORT_DB/OUT -m ../data/SHORT_SHORT_DB/META


