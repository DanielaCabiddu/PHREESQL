#!/bin/bash

echo ""
echo "Running $BASH_SOURCE"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

sqlite3 ${script_folder}/../DB/FULL_wateq4f.db < ${script_folder}/../sql_scripts/LL.sql

mv LL.csv ${script_folder}/../scratch
mv LL_ALL.csv ${script_folder}/../scratch
mv LL_Donegal.csv ${script_folder}/../scratch
mv LL_Kilkenny.csv ${script_folder}/../scratch


