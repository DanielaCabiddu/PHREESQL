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

sqlite3 < sql_scripts/ehph.sql

# CLEAN data

dos2unix scratch/eh_ph.csv

awk 'BEGIN{FS=","; OFS=","} {print "1",$2,$5,"1","nd","1","1","blue"}' scratch/eh_ph.csv > scratch/ehph.csv

# PhreePlot: change pdf keyword to TRUE TRUE flags of PLOT section in pp.set files (system folder)

$PHREEPLOT_PATH/pp sql_scripts/pp_template.ppi

