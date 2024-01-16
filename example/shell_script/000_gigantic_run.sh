#!/bin/bash

# USAGE source 000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ ${db}.dat EPA_project
#       source 000_gigantic_run.sh nfile dataset job phr_db project

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
echo $dir

export nfile=$1
export dataset=$2
export job=$3
export database=$4 # explicitly declared > phreeqc.dat
export project=$5
export db="${database%%.*}" #> phreeqc

onfile=onlydata.dat


#
# PREPROCESSING DATA - nfile untouched > split_*.dat > onlydata.dat
#
# LABELS cols 1-7
awk -F ";" ' {for(i=1;i<=7;i++) printf $i";"; print ""}' $dir/../${project}/${nfile} | sed 's/;$//g' > $dir/../${project}/scratch/split_labels.dat
 
# DATE col 8
awk 'FS=";" {print $8}' $dir/../${project}/${nfile} | awk 'FS="/" {print $3,$2,$1}' | awk '{if (($3 >= 90) && ($3 <= 99)) print "19"$1"/"$2"/"$3; else print "20"$1"/"$2"/"$3}' > $dir/../${project}/scratch/split_date.dat

# ANALYSES cols 8-47 column range **** be careful to last column *****
awk -F ";" ' {for(i=9;i<=52;i++) printf $i";"; print ""}' $dir/../${project}/${nfile} | sed 's/;$//g' > $dir/../${project}/scratch/split_analyses.dat

paste -d ";" $dir/../${project}/scratch/split_labels.dat $dir/../${project}/scratch/split_date.dat $dir/../${project}/scratch/split_analyses.dat | sed '1 d' > $dir/../${project}/scratch/${onfile}

#
source $dir/00_do_preproc.sh ${onfile}

#

source $dir/00_do_prepare.sh 
