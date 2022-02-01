#!/bin/bash

mkdir -p build
cd build

rm -rf db.db

cmake ../phreesqltool
make

./phreesql -d db_example.db -i ../data/SHORT_SHORT_DB/IN -o ../data/SHORT_SHORT_DB/OUT -m ../data/SHORT_SHORT_DB/META # --export_input --export_output --export_folder ./


