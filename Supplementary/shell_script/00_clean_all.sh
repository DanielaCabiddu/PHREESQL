#!/bin/bash

# 00_clean_all_db.sh

DIR=$PWD

# Deletes all SQL databases
rm -fr $PWD/DB/*db

# Deletes all scratch files created during computations 
rm $PWD/scratch/*

# Deletes all IN/* OUT/* and META/* files
rm -fr $PWD/run_DB/*