#!/bin/bash

# 00_create_all_input.sh SHORT [phreeqc|llnl|minteq|wateq4f|sit|ALL]


SQLDB=$1
all=$2
export dir=.

export phrsql=${dir}/phreesql
export db=/Users/zucco/softwarehouse/phreeqcrm-3.7.3-15968/build/database
export data=./run_DB/${SQLDB}


if [ -z "$2" ]
then
  echo "Specifica almeno un database di PHREEQC"
    elif  [ "$2" == "ALL" ] 
  then
    all="phreeqc llnl minteq wateq4f sit"
  else
    echo "Eseguo $2"
fi


for PHREEQCDB in ${all}
do

time ${phrsql} --run_phreeqc --phreeqc_db ${db}/${PHREEQCDB}.dat -i ${data}/IN/${PHREEQCDB} -o ${data}/OUT/${PHREEQCDB} -m ${data}/META/${PHREEQCDB} 

done
rm phreeqc.*.err

all=""