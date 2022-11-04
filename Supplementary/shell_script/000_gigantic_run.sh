#!/bin/bash

# USAGE source 000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ ${db}.dat
#       source 000_gigantic_run.sh nfile dataset job phr_db


export nfile=$1
export dataset=$2
export job=$3
export database=$4 # explicitly declared > phreeqc.dat
export db="${database%%.*}" #> phreeqc

onfile=onlydata.dat


#
# PREPROCESSING DATA - nfile untouched > split_*.dat > onlydata.dat
#
# LABELS cols 1-6
awk -F ";" ' {for(i=1;i<=6;i++) printf $i";"; print ""}' ./EPA_data/${nfile} | sed 's/;$//g' > scratch/split_labels.dat
 
# DATE col 7
awk 'FS=";" {print $7}' ./EPA_data/${nfile} | awk 'FS="/" {print $3,$2,$1}' | awk '{if (($1 >= 90) && ($1 <= 99)) print "19"$1"/"$2"/"$3; else print "20"$1"/"$2"/"$3}' > scratch/split_date.dat

# ANALYSES cols 8-46
awk -F ";" ' {for(i=8;i<=46;i++) printf $i";"; print ""}' ./EPA_data/${nfile} | sed 's/;$//g' > scratch/split_analyses.dat

paste -d ";" scratch/split_labels.dat scratch/split_date.dat scratch/split_analyses.dat | sed '1 d' > scratch/${onfile}

#
source shell_script/00_do_preproc.sh ${onfile}

#
source shell_script/00_do_prepare.sh 
