#!/bin/bash

# USAGE source EPA_project.sh EPA_project phreeqc.dat EPA_GW_IE_short.csv SHORT

prj=$1
phr_db=$2
dbshort="${phr_db%%.*}"
filein=$3
sizedb=$4

source 00_clean_all.sh ${prj}

source 00_prepare_folders.sh ${prj}

source 000_gigantic_run.sh ${filein} ${sizedb} EPA_ ${dbshort}.dat ${prj}

source 00_create_all_output.sh ${sizedb} ${dbshort} ${prj}

source 00_fill_all_db.sh ${sizedb} ${dbshort} ${prj}
