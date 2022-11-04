#!/bin/bash

# USAGE: 02_cs2cs.sh -g
#	-g upload GRASS vector
#	

echo DECLARE script variable
# DECLARE VARIABLES to be used in SQLite
export SQL_DB=$1

echo EXTRACT data from SQL db following SQL query
# EXTRACTS positional data of ZnSiO3 saturation index
sqlite3 < cs2cs.sql


echo CONVERT coordinates from EPSG TM65 to EPSG 2157
# CONVERT coordinates from 29902 (TM65) to IRENET95 (2157)
cat cs2cs.csv | sed 's/ /_/g' | awk 'BEGIN{FS="|"; OFS=","} {print $5,$6,0,$1,$2,$3,$4,$7,$8}' | sed 's/,/ /g' | \
cs2cs +init=epsg:29902 +to +init=epsg:2157 | sed -e 's/\t/,/g' -e 's/ /,/g' | \
awk ' BEGIN{FS=","; OFS="|"} {print $5,$6,$8,$1,$2,$9}' > cs2cs_2.coord

echo PREPARE data for INSERT in TABLE
awk 'BEGIN{FS="|"; OFS=","} {print $1,$2,$4,$5}' cs2cs_2.coord > IRENET95_data.dat

echo DECLARE GRASS variables
# DECLARE GRASS VARIABLES

echo UOPLOAD vector in GRASS
# UPLOAD a vector if the script run inside GRASS
#v.in.ascii in=cs2cs_2.coord out=znsio3 x=4 y=5 cols='ID integer, SITE_NAME varchar(50), SI double precision, COORD_X double precision, COORD_Y double precision, MINERAL varchar(20)'

echo CREATE TABLE in DB and INSERT data
# TODO CREATE TABLE in DB

sqlite3 /Users/zucco/Desktop/PHREESQLib/SQLDB/wateq4f_FULL.db << END_SQL
.separator ","

CREATE TABLE IF NOT EXISTS IRENET95 (
ID integer PRIMARY KEY,
SITE_NAME text,
COORD_X double,
COORD_Y double
);

.import IRENET95_data.dat IRENET95

END_SQL

echo END ===========================================
