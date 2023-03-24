#!/bin/bash

(
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR=${SCRIPT_DIR}/build
EXE_FILE=${BUILD_DIR}/phreesql
DB_FILE=${BUILD_DIR}/db_example.db

if [ ! -f ${EXE_FILE} ]
then
	${SCRIPT_DIR}/build.sh
fi

if [ -f ${DB_FILE} ]
then
	rm -f ${DB_FILE}
fi

${EXE_FILE} --fill_db -d ${DB_FILE} -i ${SCRIPT_DIR}/data/SHORT_SHORT_DB/IN -o ${SCRIPT_DIR}/data/SHORT_SHORT_DB/OUT -m ${SCRIPT_DIR}/data/SHORT_SHORT_DB/META

# --export_input --export_output --export_metadata --export_folder export_test --export_id 10 #--export_list_ids file_list.txt
# --run_phreeqc --phreeqc_db /home/danielacabiddu/Devel/src/PHREESQ/llnl.dat

##./phreesql --epsg_convert --database db_example.db --epsg 3003 --out_filename epsg_3003.csv --out_table table_epsg_3003 --out_database db_epsg_3003.db

)


