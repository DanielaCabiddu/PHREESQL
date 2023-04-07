#!/bin/bash

echo ""
echo "Running $BASH_SOURCE"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

sqlite3 ${script_folder}/../DB/FULL_ALL.db < ${script_folder}/../sql_scripts/SI_compare_ALL.sql

mv SI_chal_phreeqc.csv ${script_folder}/../scratch
mv SI_chal_llnl.csv ${script_folder}/../scratch
mv SI_chal_minteq.csv ${script_folder}/../scratch
mv SI_chal_sit.csv ${script_folder}/../scratch
mv SI_chal_wateq4f.csv ${script_folder}/../scratch
