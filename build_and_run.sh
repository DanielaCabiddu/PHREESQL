#!/bin/bash

mkdir -p build
cd build

rm -rf db.db

cmake ../phreesqltool
make

mkdir -p ../data/SHORT_SHORT_DB/OUT_phreeqc

./phreesql -d db_example.db -i ../data/SHORT_SHORT_DB/IN -o ../data/SHORT_SHORT_DB/OUT -m ../data/SHORT_SHORT_DB/META 
# --export_input --export_output --export_folder ./ --export_id 999 --export_list_ids file_list.txt
# --run_phreeqc --phreeqc_db /home/danielacabiddu/Devel/src/PHREESQ/llnl.dat


