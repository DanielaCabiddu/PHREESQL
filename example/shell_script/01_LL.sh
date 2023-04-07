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

sqlite3 $DATABASE < ${script_folder}/../sql_scripts/LL.sql

mv LL.csv ${script_folder}/../scratch
mv LL_ALL.csv ${script_folder}/../scratch
mv LL_Donegal.csv ${script_folder}/../scratch
mv LL_Kilkenny.csv ${script_folder}/../scratch


