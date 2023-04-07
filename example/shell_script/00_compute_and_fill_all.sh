#!/bin/bash

# 00_compute_and_fill_all.sh


source shell_script/00_clean_all.sh 

source shell_script/00_prepare_folders.sh 

echo "Build folder structure"

sleep 1

for outer in SHORT MEDIUM FULL
do

  for inner in phreeqc llnl minteq wateq4f sit
   do

case "$outer" in
SHORT)
nfile=EPA_GW_IE_short.csv
;;
MEDIUM)
nfile=EPA_GW_IE_medium.csv
;;
FULL)
nfile=EPA_GW_IE.csv
;;
esac

echo "FILE: $nfile , DATASET:  $outer , DATABASE: $inner"
echo "Build input files $nfile $dt $i $j"
source shell_script/000_gigantic_run.sh $nfile $outer EPA_ $inner.dat

echo "Compute PHREEQC output"
source shell_script/00_create_all_output.sh $outer $inner

echo "Fill SQL databases"
source shell_script/00_fill_all_db.sh $outer $inner

 done
done