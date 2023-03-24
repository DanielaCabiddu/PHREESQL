#!/bin/bash

(
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR=${SCRIPT_DIR}/build

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

rm -rf *
cmake ../phreesqltool
make

cp $PWD/proj/data/proj.db $PWD/
ln -sf $PWD/phreesql $PWD/../Supplementary/phreesql

./phreesql --fill_db -d db_example.db -i ../data/SHORT_SHORT_DB/IN -o ../data/SHORT_SHORT_DB/OUT -m ../data/SHORT_SHORT_DB/META
)

# --export_input --export_output --export_metadata --export_folder export_test --export_id 10 #--export_list_ids file_list.txt
# --run_phreeqc --phreeqc_db /home/danielacabiddu/Devel/src/PHREESQ/llnl.dat

##./phreesql --epsg_convert --database db_example.db --epsg 3003 --out_filename epsg_3003.csv --out_table table_epsg_3003 --out_database db_epsg_3003.db
