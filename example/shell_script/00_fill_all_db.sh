#!/bin/bash

# 00_fill_all_db.sh MEDIUM [phreeqc|llnl|minteq|wateq4f|sit|ALL]

SQLDB=$1
DB=$2

if [ -z "$2" ]
then
  echo "Specifica almeno un database di PHREEQC"
    elif  [ "$2" == "ALL" ] 
  then
    all="phreeqc llnl minteq wateq4f sit"
  else
    echo "Eseguo $2"
    all=$2
fi

export dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

export phrsql=${dir}/../../build/phreesqlexe
export db=/Users/zucco/softwarehouse/phreeqcrm-3.7.3-15968/build/database
export data=./run_DB/${SQLDB}

echo inizio loop

for PHREEQCDB in ${all}
do
echo
echo "========= ${PHREEQCDB} ==========="

if [  ${DB} = ALL  ] 
then
 echo "Vengono creati i database SQLDB_PHREEQCDB  ; es. FULL_minteq.db"
 PHREEQCDBALL=${DB}

    else
 PHREEQCDBALL=${PHREEQCDB}
 echo "Viene creato 1 database SQLDB_PHREEQCDBALL ; es. FULL_ALL.db "
 
fi
echo fine loop

time ${phrsql} --fill_db -d ./DB/${SQLDB}_${PHREEQCDBALL}.db -i ${data}/IN/${PHREEQCDB} -o ${data}/OUT/${PHREEQCDB} -m ${data}/META/${PHREEQCDB} 

done

#all=""
rm phreeqc.*.err
