#/bin/bash

export ver=20221012
export PHREEPLOT=$HOME/softwarehouse/pp-$ver/phreeplot
export PHREEPLOT_PATH=$PHREEPLOT/bin

if [[ $PATH =~ "$ver" ]]; then
 echo -n "
 Phreeplot home is already in PATH global variable\n
 
 "
   else
 export PATH="$PATH:$PHREEPLOT_PATH"
fi

# EXTRACTS data from db

sqlite3 < sql_scripts/ehph_donegal.sql

dos2unix scratch/ehph_donegal.csv

awk 'BEGIN{FS=","; OFS=","} {print "1",$2,$5,"1","nd","1","1","blue"}' scratch/ehph_donegal.csv > scratch/ehph.csv


sqlite3 < sql_scripts/ehph_kilkenny.sql

dos2unix scratch/ehph_kilkenny.csv

awk 'BEGIN{FS=","; OFS=","} {print "1",$2,$5,"1","nd","1","1","red"}' scratch/ehph_kilkenny.csv >> scratch/ehph.csv

#dos2unix scratch/eh_ph.csv


# PhreePlot: change pdf keyword to TRUE TRUE flags of PLOT section in pp.set files (system folder)

$PHREEPLOT_PATH/pp sql_scripts/pp_template.ppi

