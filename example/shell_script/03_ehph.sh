#/bin/bash

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

script_folder=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
DATABASE=$1


export PHREEPLOT_PATH=${script_folder}/../bin
export PHREEPLOT=${PHREEPLOT_PATH}/phreeplot
export PHREEPLOT_EXE=${PHREEPLOT}/bin/pp

if [ ! -f ${PHREEPLOT_EXE} ]
then

	mkdir -p ${PHREEPLOT_PATH}

	if [ "$(uname)" == "Darwin" ]; then
		# Do something under Mac OS X platform   
		
		wget --no-check-certificate --no-proxy http://www.phreeplot.org/binaries/Mac/pp_1-Darwin-gfortran_11_12.3_arm64_15968.tar.bz2
		mv pp_1-Darwin-gfortran_11_12.3_arm64_15968.tar.bz2 ${PHREEPLOT_PATH}
		tar -xvf ${PHREEPLOT_PATH}/pp_1-Darwin-gfortran_11_12.3_arm64_15968.tar.bz2 -C ${PHREEPLOT_PATH}
		     
	elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
		# Do something under GNU/Linux platform

		LINUX_OS=`lsb_release -d | awk -F"\t" '{print $2}' | awk -F" " '{print $1}'`
		
		echo ${LINUX_OS}
		
		if [ "${LINUX_OS}" == "Ubuntu" ]; then
			wget --no-check-certificate --no-proxy https://phreeplot.org/binaries/Linux/pp_1-Linux-gfortran_10_Ubuntu_x86_64_15968.tar.bz2
			mv pp_1-Linux-gfortran_10_Ubuntu_x86_64_15968.tar.bz2 ${PHREEPLOT_PATH}
			tar -xvf ${PHREEPLOT_PATH}/pp_1-Linux-gfortran_10_Ubuntu_x86_64_15968.tar.bz2 -C ${PHREEPLOT_PATH}

		elif [ "${LINUX_OS}" == "Centos" ]; then	### Check
		
			wget --no-check-certificate --no-proxy https://www.phreeplot.org/binaries/Linux/pp_1-Linux-gfortran_11_Mint_x86_64_15968.tar.bz2
			mv pp_1-Linux-gfortran_11_Mint_x86_64_15968.tar.bz2 ${PHREEPLOT_PATH}
			tar -xvf ${PHREEPLOT_PATH}/pp_1-Linux-gfortran_11_Mint_x86_64_15968.tar.bz2 -C ${PHREEPLOT_PATH}

		elif [ "${LINUX_OS}" == "OpenSuse" ]; then	### Check
		
			wget --no-check-certificate --no-proxy https://www.phreeplot.org/binaries/Linux/pp_1-Linux-gfortran_11.2_Suse_x86_64_15968.tar.bz2
			mv pp_1-Linux-gfortran_11.2_Suse_x86_64_15968.tar.bz2 ${PHREEPLOT_PATH}
			tar -xvf ${PHREEPLOT_PATH}/pp_1-Linux-gfortran_11.2_Suse_x86_64_15968.tar.bz2 -C ${PHREEPLOT_PATH}
		
		else	
			"Unsupported Linux OS ${LINUX_OS} : No Phreeplot Binary available."
			exit 1
		fi
	else

		echo "Unsupported $(uname) : No Phreeplot Binary available."
		exit 1
	fi
fi

# EXTRACTS data from db

sqlite3 $DATABASE < ${script_folder}/../sql_scripts/ehph_donegal.sql
mv ehph_donegal.csv ${script_folder}/../scratch/

dos2unix ${script_folder}/../scratch/ehph_donegal.csv

awk 'BEGIN{FS=","; OFS=","} {print "1",$2,$5,"1","nd","1","1","blue"}' ${script_folder}/../scratch/ehph_donegal.csv > ${script_folder}/../scratch/ehph.csv


sqlite3 $DATABASE < ${script_folder}/../sql_scripts/ehph_kilkenny.sql
mv ehph_kilkenny.csv ${script_folder}/../scratch/

dos2unix ${script_folder}/../scratch/ehph_kilkenny.csv

awk 'BEGIN{FS=","; OFS=","} {print "1",$2,$5,"1","nd","1","1","red"}' ${script_folder}/../scratch/ehph_kilkenny.csv >> ${script_folder}/../scratch/ehph.csv

#dos2unix scratch/eh_ph.csv


# PhreePlot: change pdf keyword to TRUE TRUE flags of PLOT section in pp.set files (system folder)

$PHREEPLOT_EXE ${script_folder}/../sql_scripts/pp_template.ppi
