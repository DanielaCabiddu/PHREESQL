#!/bin/bash

# USAGE: 02_cs2cs.sh 
	
# if $1 == g else you are in GRASS
echo DECLARE script variable
# DECLARE VARIABLES to be used in SQLite
export SQL_DB=$1

./phreesql --epsg_convert --database DB/FULL_wateq4f.db --epsg 2157 --out_database DB/FULL_wateq4f_2157.db --out_filename scratch/FULL_wateq4f_2157.csv

sqlite3 < sql_scripts/cs2cs.sql

# if -g then
# v.in.ascii sep=';' in=scratch/cs2cs.csv out=SI_cc x=5 y=6 skip=1 columns='site varchar(49), ID integer, name varchar(40), date varchar(10), east=double precision, north double precision, si_c double precision, phase varchar(7)'


echo END ===========================================
