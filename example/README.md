# Case Study

The application below has been designed to show how PHREESQL can be employed. Through some bash scripts, the complete pipeline for pre-processing input files, computing solution speciation, creating various SQL databases, and extracting data is demonstrated. The examples are based on a large dataset of publicly accessible geochemical data, covering the generation and post-processing of PHREEQC output data. Specifically, the [Ireland EPA database](https://gis.epa.ie/GetData/Download, filename: EPA Groundwater Monitoring Data to End 2020 Circulation 26.05.21.xlsx) is used as an input dataset.The dataset contains a total of 295 sampling stations in Ireland’s aquifers, which have been sampled for a total of 14690 analyses over a 30-year period until 2020. A total of 303 variables are present in the original dataset, which provides descriptive information, spatial coordinates, dissolved components or species, and chlorinated organic compounds. The file utilized as an input in our experimental pipeline is a modified iteration of the original Ireland EPA dataset. This revised version consists of 45 columns and excludes the notably sparse organic compound records. Both the original and the modified versions of the input dataset can be found in the **EPA_project** folder, assumed to be the designated project for this case study.

As a matter of example and no loss of generality, we demonstrate the usage of PHREESQL by running PHREESQLexe, which is assumed to be located in the **${ROOT}/build** folder in this repository. The outlined steps focus on data extraction, data plotting, and integration with other software, including coordinate conversion for variographic study and mapping of a geochemical variable as output of PHREEQC computations. The entire procedure, implemented in scripts, is available in the distribution and can be evaluated at each step.

## PHREESQL database creation

The script *00_step-by-step.sh* consolidates all the PHREESQL capabilities into one, also assisting the reader in PHREEQC input generation starting from the *.csv* file. It sequentially calls the following scripts

- [shell_script/00_clean_all.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_clean_all.sh)
- [shell_script/00_prepare_folders.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_prepare_folders.sh)
- [shell_script/000_gigantic_run.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/000_gigantic_run.sh)
- [shell_script/00_create_all_output.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_create_all_output.sh)
- [shell_script/00_fill_all_db.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_fill_all_db.sh)

Further information on each individual script is elaborated in the sections below.

### shell_script/00_clean_all.sh
This script is responsible for the cleanup of files within the **EPA_project/DB**, **EPA_project/scratch** and **EPA_project/run_DB** folders.

### shell_script/00_prepare_folders.sh
This script is designed to prepare the folder structure within **EPA_project/run_DB** to accommodate *.pqi*, *.pqo*, and *.met* files.

### shell_script/000_gigantic_run.sh
This script is responsible for preparing data subsets and organizing PHREEQC thermodynamic databases. Derived from the modified Ireland EPA dataset, this script creates three distinct data subsets (*SHORT*, *MEDIUM* and *FULL*) and organizes the data for speciation simulations using five PHREEQC thermodynamic databases (*llnl.dat*, *wateq4f.dat*, *phreeqc.dat*, *minteq.dat* and *sit.dat*). For each combination of data subset *D* and PHREEQC thermodynamic database *P*, the script generates PHREEQC input files *(.pqi)* and corresponding metadata files *(.met)*. These files are then stored in the following directories: **EPA_project/run_DB/${D}/IN/${P}** and **EPA_project/run_DB/${D}/MET/${P}**.

### shell_script/00_create_all_output.sh
This script calculates parallel speciation simulations for each combination of data subset *D* and PHREEQC thermodynamic database *P*. This is achieved by sequentially invoking PHREESQLexe with the following command line:

`${ROOT}/build/phreesqlexe --run_phreeqc --phreeqc_db ${P} --in_folder EPA_project/run_DB/${D}/IN/${P} --out_folder EPA_project/run_DB/${D}/OUT/${P}`

### shell_script/00_fill_all_db.sh
This scripts creates and fills PHREESQL databases, one for each combination of data subset *D* and PHREEQC thermodynamic database *P*. This is achieved by sequentially invoking PHREESQLexe with the following command line:

`${ROOT}/build/phreesqlexe --fill_db -d EPA_project/DB/${D}_${P}.db -i EPA_project/run_DB/${D}/IN/${P} -o EPA_project/run_DB/${D}/OUT/${P} -m EPA_project/run_DB/${D}/MET/${P}`

## PHREESQL database query

- [shell_script/01_LL.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/01_LL.sh)
- [shell_script/02_SI.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/02_SI.sh)
- [shell_script/03_ehph.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/03_ehph.sh)
- [shell_script/04_SI_compare_ALL.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script04_SI_compare_ALL.sh)
- [shell_script/05_cs2cs.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/05_cs2cs.sh)

### shell_script/01_LL.sh
This script demonstrates how to query a PHREESQL database generated by our case study to extract groundwater samples. Specifically, the SQL script *LL.sql*, called by the *LL.sh* bash script, is used to extract groundwater samples from the *FULL_wateq4f.db* PHREESQL database, located in the **EPA_project/DB** folder. The filter applied in this script is based on the county’s name, allowing users to retrieve data from specific counties without applying additional compositional or temporal filters. The output consists of three ASCII files, one for the whole dataset () and two for a pair of Irish counties (Kilkenny and Donegal, arbitrarily chosen). By customizing the query, looping over all the 26 counties, the LL diagram can present an atomic information about each County.

### shell_script/02_SI.sh

sql_scripts/SI.sql

### shell_script/03_ehph.sh
This script is designed to illustrate the process of calling an external package, specifically PhreePlot, to create a graphical output of PHREESQL database queries. The SQL script *03_eh_ph.sh*, called by the *03_ehph.sh* bash script, extracts the *pe - pH* couples from the *FULL_wateq4f.db* PHREESQL database, located in the **EPA_project/DB** folder for both Kilkenny and Donegal counties, as previously done, with a single constraint on charge balance within a range of +/-10%.

sql_scripts/ehph.sql

### shell_script/04_SI_compare_ALL.sh

sql_scripts/VARIO.sql

### shell_script/05_cs2cs.sh

sql_scripts/cs2cs.sql
