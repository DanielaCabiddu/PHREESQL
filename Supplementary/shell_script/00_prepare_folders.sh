#!/bin/bash

# 00_prepare_folders.sh minteq

dir=.

phrsql=${dir}/phreesql
db=/Users/zucco/softwarehouse/phreeqcrm-3.7.3-15968/build/database
data=./run_DB/${SQLDB}

all="phreeqc llnl minteq wateq4f sit"

for dataset in SHORT MEDIUM FULL
do

folder="run_DB/$dataset"

# if [ -z $folder ] 
# then
   mkdir -p run_DB/$dataset
     for canister in IN OUT META
     do
       for pdb in $all
       do
         mkdir -p $folder/$canister/$pdb
       done
     done 
# fi
done