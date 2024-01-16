#! /bin/bash

#USAGE: 00_step-by-step_PERFORMANCE.sh EPA_project
#       00_step-by-step_PERFORMANCE.sh project_name
# prj=project_name

rm file*.txt performance.txt

prj=$1
databases="phreeqc llnl minteq wateq4f sit"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

echo "# ######################################################################### #
#          Clean a previous computation of the project_name
# ######################################################################### #" > performance.txt

 source $script_folder/shell_script/00_clean_all.sh 
 source $script_folder/shell_script/00_prepare_folders.sh 


echo "# ######################################################################### #
#   Preprocess the .csv file with water analises in the project_name
# ######################################################################### #" >> performance.txt

################################################################ SHORT
for phrqc_db in ${databases}
do

start=`date +%s`
source $script_folder/shell_script/000_gigantic_run.sh EPA_GW_IE_short.csv SHORT EPA_ ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:SHORT Execution time was `expr $end - $start` seconds. >> performance.txt
done

################################################################ MEDIUM
for phrqc_db in ${databases}
do
start=`date +%s`
source $script_folder/shell_script/000_gigantic_run.sh EPA_GW_IE_medium.csv MEDIUM EPA_ ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:MEDIUM Execution time was `expr $end - $start` seconds. >> performance.txt
done

############################################################### FULL
for phrqc_db in ${databases}
do
start=`date +%s`
source $script_folder/shell_script/000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:FULL Execution time was `expr $end - $start` seconds. >> performance.txt
done

echo "# ######################################################################### #
#   PHREEQC output are computed in parallel execution
# ######################################################################### #" >> performance.txt


################################################################ SHORT
for phrqc_db in ${databases}
do
start=`date +%s`
source $script_folder/shell_script/00_create_all_output.sh SHORT ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:SHORT Execution time was `expr $end - $start` seconds. >> performance.txt
done

################################################################ MEDIUM
for phrqc_db in ${databases}
do
start=`date +%s`
source $script_folder/shell_script/00_create_all_output.sh MEDIUM ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:MEDIUM Execution time was `expr $end - $start` seconds. >> performance.txt
done

################################################################ FULL
for phrqc_db in ${databases}
do
start=`date +%s`
source $script_folder/shell_script/00_create_all_output.sh FULL ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:FULL Execution time was `expr $end - $start` seconds. >> performance.txt
done

echo "# ######################################################################### #
#   Creates and fill SQL databases with IN - OUT - META dataset
# ######################################################################### #" >> performance.txt


# SHORT: 42 / MEDIUM: 4354 / FULL: 14690
################################################################ SHORT
for phrqc_db in ${databases}
do
start=`date +%s`
source $script_folder/shell_script/00_fill_all_db.sh SHORT ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:SHORT Execution time was `expr $end - $start` seconds. >> performance.txt
done

################################################################ MEDIUM
for phrqc_db in ${databases}
do
start=`date +%s`
source $script_folder/shell_script/00_fill_all_db.sh MEDIUM ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:MEDIUM Execution time was `expr $end - $start` seconds. >> performance.txt
done

################################################################ FULL
for phrqc_db in ${databases}
do
start=`date +%s`
source $script_folder/shell_script/00_fill_all_db.sh FULL ${phrqc_db} ${prj}
end=`date +%s`
echo DATABASE: $phrqc_db  SIZE:FULL Execution time was `expr $end - $start` seconds. >> performance.txt
done

echo "# ######################################################################### #
#   Creates and fill a sigle SQL database with IN  - OUT - META dataset 
#         computed with all previous thermodynamical databases
# ######################################################################### #" >> performance.txt


################################################################ ALL
for size in SHORT MEDIUM FULL
do
start=`date +%s`
source $script_folder/shell_script/00_fill_all_db.sh $size  ALL $prj
end=`date +%s`
echo  DATABASE: ALL $size Execution time was `expr $end - $start` seconds. >> performance.txt
done

echo " ######################### DONE ###############################"