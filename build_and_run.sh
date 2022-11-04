#!/bin/bash

mkdir -p build
cd build

rm -rf db.db
cmake ../phreesqltool
make

cp $PWD/proj/data/proj.db $PWD/

ln -sf $PWD/phreesql $PWD/../Supplementary/phreesql

mkdir -p ../data/SHORT_SHORT_DB/OUT_phreeqc

./phreesql --fill_db -d db_example.db -i ../data/SHORT_DB/IN/llnl -o ../data/SHORT_DB/OUT/llnl -m ../data/SHORT_DB/META/llnl
# --export_input --export_output --export_metadata --export_folder export_test --export_id 10 #--export_list_ids file_list.txt
# --run_phreeqc --phreeqc_db /home/danielacabiddu/Devel/src/PHREESQ/llnl.dat

./phreesql --epsg_convert --database db_example.db --epsg 3003 --out_filename epsg_3003.csv --out_table table_epsg_3003 --out_database db_epsg_3003
