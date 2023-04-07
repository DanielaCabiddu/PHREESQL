#!/bin/bash

# 00_clean_all_db.sh

echo ""
echo "Running $BASH_SOURCE"

DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Deletes all SQL databases
rm -fr $DIR/../DB

# Deletes all scratch files created during computations 
rm -fr $DIR/../scratch

# Deletes all IN/* OUT/* and META/* files
rm -fr $DIR/../run_DB
