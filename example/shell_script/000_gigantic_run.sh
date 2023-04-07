#!/bin/bash

# USAGE source 000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ ${db}.dat
#       source 000_gigantic_run.sh nfile dataset job phr_db

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

export nfile=$1
export dataset=$2
export job=$3
export database=$4 # explicitly declared > phreeqc.dat
export db="${database%%.*}" #> phreeqc

onfile=onlydata.dat


#
# PREPROCESSING DATA - nfile untouched > split_*.dat > onlydata.dat
#
# LABELS cols 1-7
awk -F ";" ' {for(i=1;i<=7;i++) printf $i";"; print ""}' $dir/../EPA_data/${nfile} | sed 's/;$//g' > $dir/../scratch/split_labels.dat
 
# DATE col 8
awk 'FS=";" {print $8}' $dir/../EPA_data/${nfile} | awk 'FS="/" {print $3,$2,$1}' | awk '{if (($1 >= 90) && ($1 <= 99)) print "19"$1"/"$2"/"$3; else print "20"$1"/"$2"/"$3}' > $dir/../scratch/split_date.dat

# ANALYSES cols 8-46
awk -F ";" ' {for(i=9;i<=46;i++) printf $i";"; print ""}' $dir/../EPA_data/${nfile} | sed 's/;$//g' > $dir/../scratch/split_analyses.dat

paste -d ";" $dir/../scratch/split_labels.dat $dir/../scratch/split_date.dat $dir/../scratch/split_analyses.dat | sed '1 d' > $dir/../scratch/${onfile}

#
source $dir/00_do_preproc.sh ${onfile}

#
source $dir/00_do_prepare.sh 
