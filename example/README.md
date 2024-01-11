# Examples

The application below has been designed to show how PHREESQL can be employed. Through some bash scripts the complete pipeline for pre-processing input files, computing solution speciation, creating various SQL databases and extracting data is demonstrated. The examples are based on a large dataset of publicly accessible geochemical data, covering the generation and post-processing of PHREEQC output data.

The outlined steps focus on data extraction, data plotting, and integration with other software, including coordinate conversion for variographic study and mapping. The entire procedure, implemented in scripts, is available in the distribution and can be evaluated at each step.

As a matter of example and no loss of generality, we demonstrate the usage of PHREESQL by running PHREESQLexe, which is assumed to be located in the **${ROOT}/build** folder in this repository.

## How to create PHREESQL databases

The script *00_step-by-step.sh* consolidates all the PHREESQL capabilities into one, also assisting the reader in PHREEQC input generation starting from the .csv file. It enables the possibility to run the full pipeline on a user-defined project, provided as an input parameter, by sequentially calling the following bash scripts:

- [shell_script/00_clean_all.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_clean_all.sh)
- [shell_script/00_prepare_folders.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_prepare_folders.sh)
- [shell_script/000_gigantic_run.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/000_gigantic_run.sh)
- [shell_script/00_create_all_output.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_create_all_output.sh)
- [shell_script/00_fill_all_db.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_fill_all_db.sh)
- [shell_script/00_compute_and_fill.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/00_compute_and_fill.sh)

Details on each single script are provided in the sections below.



### shell_script/00_clean_all.sh

/* Script cleaning all files in DB, scratch and run_DB*/

[no argument]

### shell_script/00_prepare_folders.sh 

/* Script preparing run_DB folder structure to accept .pqi, .pqo and .met files*/

[no argument] 

### shell_script/000_gigantic_run.sh 

/* Script to generate from a .csv dataset a SHORT|MEDIUM|FULL database based on short dataset with a minted PHREEQC thermodynamic database (with extension) and naming all input and output files with EPA_ prefix*/

EPA_GW_IE_short.csv SHORT|MEDIUM|FULL EPA_ minteq.dat|phreeqc.dat|sit.dat|wateq4f.dat|llnl.dat  

### shell_script/00_create_all_output.sh 

/* Script to compute parallel speciation simulations */

SHORT|MEDIUM|FULL phreeqc|llnl|minteq|wateq4f|sit|ALL

### shell_script/00_fill_all_db.sh 

/* Script to fill database having already .pqi, .met and .pqo files from previous script */

SHORT|MEDIUM|FULL phreeqc|llnl|minteq|wateq4f|sit|ALL

### shell_script/00_compute_and_fill.sh 

/* Script to generate all database, from all datasets */

[no argument]


## How to query PHREESQL databases

- [shell_script/01_LL.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/01_LL.sh)
- [shell_script/02_SI.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/02_SI.sh)
- [shell_script/03_ehph.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/03_ehph.sh)
- [shell_script/04_SI_compare_ALL.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script04_SI_compare_ALL.sh)
- [shell_script/05_cs2cs.sh](https://github.com/DanielaCabiddu/PHREESQL/blob/main/example/shell_script/05_cs2cs.sh)

### shell_script/01_LL.sh
The LL plot (Langelier and Ludwig, 1942) is a graphical tool used to classify waters based on the chemistry of major cations and anions in percent of meq/l. Categorizing groundwater based on various criteria and applying filters can help users analyze and interpret data more effectively. Criteria can include rock type, county name, aquifer name, or time interval, and these categories can be plotted separately for better visualization and analysis. Filters can involve selecting specific time ranges or applying conditions related to electrical balance or concentration ranges to refine the dataset and focus on specific aspects of groundwater data.

The SQL script *LL.sql*, called by the *LL.sh* bash script, is used to extract groundwater samples from the *FULL_wateq4f.db* SQL database. The filter applied in this script is based on the county’s name, allowing users to retrieve data from specific counties without applying additional compositional or temporal filters. The output consists of three ASCII files, one for the whole dataset and two for a pair of Irish counties (Kilkenny and355 Donegal, arbitrarily chosen).

### shell_script/02_SI.sh

sql_scripts/SI.sql

### shell_script/03_ehph.sh

sql_scripts/ehph.sql

### shell_script/04_SI_compare_ALL.sh

sql_scripts/VARIO.sql

### shell_script/05_cs2cs.sh

sql_scripts/cs2cs.sql

