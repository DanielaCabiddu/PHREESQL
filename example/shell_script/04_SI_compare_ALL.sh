#!/bin/bash

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
DATABASE=$1

sqlite3 $DATABASE < ${script_folder}/../sql_scripts/SI_compare_ALL.sql

mv SI_chal_phreeqc.csv ${script_folder}/../scratch
mv SI_chal_llnl.csv ${script_folder}/../scratch
mv SI_chal_minteq.csv ${script_folder}/../scratch
mv SI_chal_sit.csv ${script_folder}/../scratch
mv SI_chal_wateq4f.csv ${script_folder}/../scratch