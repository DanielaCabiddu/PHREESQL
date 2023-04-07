#!/bin/bash

# 00_create_all_input.sh SHORT [phreeqc|llnl|minteq|wateq4f|sit|ALL]

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

SQLDB=$1
all=$2
export dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

export phrsql=${dir}/../../build/phreesqlexe
export db=${dir}/../../external/iphreeqc/database
export data=${dir}/../run_DB/${SQLDB}


if [ -z "$2" ]
then
  echo "A PHREEQC database is mandatory and must be provided."
    elif  [ "$2" == "ALL" ] 
  then
    all="phreeqc llnl minteq wateq4f sit"
  else
  	echo ""
    echo "Running PHREESQL for PHREEQC database $2"
    echo ""
fi


for PHREEQCDB in ${all}
do

time ${phrsql} --run_phreeqc --phreeqc_db ${db}/${PHREEQCDB}.dat -i ${data}/IN/${PHREEQCDB} -o ${data}/OUT/${PHREEQCDB} -m ${data}/META/${PHREEQCDB} 

done
rm phreeqc.*.err

all=""
