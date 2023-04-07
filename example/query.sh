#! /bin/bash

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
DATABASE=$1

${script_folder}/shell_script/01_LL.sh $DATABASE ####${script_folder}/../DB/FULL_wateq4f.db
${script_folder}/shell_script/02_SI.sh $DATABASE 
${script_folder}/shell_script/03_ehph.sh $DATABASE 
${script_folder}/shell_script/04_SI_compare_ALL.sh $DATABASE 
${script_folder}/shell_script/05_cs2cs.sh $DATABASE 

