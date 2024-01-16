#!/bin/bash
# USAGE: source 00_big_run.sh alpsco2_ phreeqc.dat DB1-2_measures.csv
#        source 00_big_run.sh  job       database       datafile
# job:
# database:
# datafile:

# run_DB/
# 		/IN
# 			/phreeqc
# 			/wateq4f
# 		/OUT
# 			/phreeqc
# 			/wateq4f
# 			...


# for i in wateq4f.dat phreeqc.dat llnl.dat sit.dat minteq.dat
#  do
#  source 00_big_run.sh EPA_ ${i} EPA_GW_IE.csv
# done

export job=$1			# alpsco2_	
export database=$2 # explicitly declared i.e. phreeqc.dat
export db="${database%%.*}" #> phreeqc

export fdb=$3	# test_DB1.csv

mkdir -p IN/${db} OUT/${db} META/${db}

dos2unix ${fdb}

export units="mg/l"
export wdir=`pwd`


# preprocessing
source 00_do_preproc.sh ${fdb}

echo "do_preproc END"

# file prepration
source 00_do_prepare.sh

echo "do_prepare END"


# phreeqc loop
source 00_do_phreeqc.sh

time ${phrsql} --run_phreeqc --phreeqc_db ${db}/${PHREEQCDB}.dat -i ${data}/IN/${PHREEQCDB} -o ${data}/OUT/${PHREEQCDB} -m ${data}/META/${PHREEQCDB} -d ${SQLDB}_${PHREEQCDB}.db 

echo "do_phreeqc_END"


# analysis
#source 04_do_analysis.sh

echo "do_analysis END"

# plot data
#source do_plot_phreeqc.sh "CO2Ca" "NaCl" "eh_ph" "CaMg" "alk_I"

echo "do_plot END"

# ./do_plot_phreeplot.sh


