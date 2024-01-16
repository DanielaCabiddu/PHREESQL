#!/bin/bash

# 00_fill_all_db.sh MEDIUM [phreeqc|llnl|minteq|wateq4f|sit|ALL] EPA_project

echo ""
echo "Running $BASH_SOURCE"

SQLDB=$1
DB=$2
project=$3

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
    all=$2
fi

export dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

export phrsql=${dir}/../../build/phreesqlexe
export db=${dir}/../../external/iphreeqc/database
export data=${dir}/../${project}/run_DB/${SQLDB}

for PHREEQCDB in ${all}
do
echo
echo "========= ${PHREEQCDB} ==========="

if [  ${DB} = ALL  ] 
then
 #echo "Vengono creati i database SQLDB_PHREEQCDB  ; es. FULL_minteq.db"
 PHREEQCDBALL=${DB}

    else
 PHREEQCDBALL=${PHREEQCDB}
 #echo "Viene creato 1 database SQLDB_PHREEQCDBALL ; es. FULL_ALL.db "
 
fi

time ${phrsql} --fill_db -d $dir/../${project}/DB/${SQLDB}_${PHREEQCDBALL}.db -i ${data}/IN/${PHREEQCDB} -o ${data}/OUT/${PHREEQCDB} -m ${data}/META/${PHREEQCDB} 

done

#all=""
rm phreeqc.*.err
