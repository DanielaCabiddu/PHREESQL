#!/bin/bash

# 00_prepare_folders.sh EPA_project
#                                    data_folder
# in data_folder are present the .csv files, will be generated DB, scratch, run_DB folders

export project=$1

echo ""
echo "Running $BASH_SOURCE"

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

echo $dir
#phrsql=${dir}/../../build/phreesqlexe
#db=${dir}/../../external/iphreeqc/database
#data=${dir}/../run_DB/${SQLDB}

mkdir -p $dir/../${project}/run_DB/
mkdir -p $dir/../${project}/scratch/
mkdir -p $dir/../${project}/DB/

all="phreeqc llnl minteq wateq4f sit"

for dataset in SHORT MEDIUM FULL
do

folder="$dir/../${project}/run_DB/$dataset"

# if [ -z $folder ] 
# then
   mkdir -p $folder
     for canister in IN OUT META
     do
       for pdb in $all
       do
         mkdir -p $folder/$canister/$pdb
       done
     done 
# fi
done
