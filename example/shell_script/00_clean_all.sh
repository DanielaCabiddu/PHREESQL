#!/bin/bash

# USAGE: source ./00_clean_all_db.sh EPA_project
#                                    data_folder
# in data_folder are present the .csv files, will be generated DB, scratch, run_DB folders

export project=$1

echo ""
echo "Running $BASH_SOURCE"

DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Deletes all SQL databases
rm -fr $DIR/../${project}/DB

# Deletes all scratch files created during computations 
rm -fr $DIR/../${project}/scratch

# Deletes all IN/* OUT/* and META/* files
rm -fr $DIR/../${project}/run_DB
