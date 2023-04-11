#!/bin/bash

# 00_prepare_folders.sh minteq

echo ""
echo "Running $BASH_SOURCE"

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#phrsql=${dir}/../../build/phreesqlexe
#db=${dir}/../../external/iphreeqc/database
#data=${dir}/../run_DB/${SQLDB}

mkdir -p $dir/../run_DB/
mkdir -p $dir/../scratch/
mkdir -p $dir/../DB/

all="phreeqc llnl minteq wateq4f sit"

for dataset in SHORT MEDIUM FULL
do

folder="$dir/../run_DB/$dataset"

# if [ -z $folder ] 
# then
   mkdir -p $dir/run_DB/$dataset
     for canister in IN OUT META
     do
       for pdb in $all
       do
         mkdir -p $folder/$canister/$pdb
       done
     done 
# fi
done
