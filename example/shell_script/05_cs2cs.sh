#!/bin/bash

# USAGE: 02_cs2cs.sh 

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
DATABASE=$1
	
# if $1 == g else you are in GRASS
echo DECLARE script variable
# DECLARE VARIABLES to be used in SQLite
export SQL_DB=$1

${script_folder}/../../build/phreesqlexe --epsg_convert --database $DATABASE --epsg 2157 --out_database ${script_folder}/../DB/FULL_wateq4f_2157.db --out_filename ${script_folder}/../scratch/FULL_wateq4f_2157.csv

#sqlite3 ${script_folder}/../DB/FULL_wateq4f_2157.db < ${script_folder}/../sql_scripts/cs2cs.sql

#mv cs2cs.csv ${script_folder}/../scratch

# if -g then
# v.in.ascii sep=';' in=scratch/cs2cs.csv out=SI_cc x=5 y=6 skip=1 columns='site varchar(49), ID integer, name varchar(40), date varchar(10), east=double precision, north double precision, si_c double precision, phase varchar(7)'


