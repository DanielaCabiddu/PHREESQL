#!/bin/bash

echo ""
echo "Running $BASH_SOURCE"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

sqlite3 ${script_folder}/../DB/FULL_wateq4f.db < ${script_folder}/../sql_scripts/SI.sql

mv SI_Donegal.csv ${script_folder}/../scratch
mv SI_Kilkenny.csv ${script_folder}/../scratch

