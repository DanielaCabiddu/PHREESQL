#! /bin/bash

# USAGE: source query_project.sh phreeqc FULL EPA_project
ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
phrqc_db=$1
SIZE=$2
prj=$3

DATABASE=${script_folder}/${prj}/DB/${SIZE}_${phrqc_db}.db

#  RUNNING
  ${script_folder}/shell_script/01_LL.sh $DATABASE
  ${script_folder}/shell_script/02_SI.sh $DATABASE 
  ${script_folder}/shell_script/03_ehph.sh ${phrqc_db} ${SIZE} ${prj} 
  ${script_folder}/shell_script/04_SI_compare_ALL.sh ALL ${SIZE} ${prj}
  ${script_folder}/shell_script/05_cs2cs.sh ${phrqc_db} ${SIZE} ${prj}  

