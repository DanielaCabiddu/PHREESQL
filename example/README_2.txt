README

Pre-requisite: SQLite 3.*, PHREEQCRM, proj must be installed

Bash shell scripts as reported below must be run inside that folder, otherwise scripts should be changed consistently. The source code PHREESQLIB must be at the same level of that folder. The package phreesql is the alias of the binary in building folder.

AVAILABLE SCRIPTS [with syntax]
===============================
/* Script cleaning all files in DB, scratch and run_DB*/
source shell_script/00_clean_all.sh [no argument]

/* Script preparing run_DB folder structure to accept .pqi, .pqo and .met files*/
source shell_script/00_prepare_folders.sh [no argument]

/* Script to generate from a .csv dataset a SHORT|MEDIUM|FULL database based on short dataset with a minted PHREEQC thermodynamic database (with extension) and naming all input and output files with EPA_ prefix*/
source shell_script/000_gigantic_run.sh EPA_GW_IE_short.csv SHORT|MEDIUM|FULL EPA_ minteq.dat|phreeqc.dat|sit.dat|wateq4f.dat|llnl.dat

/* Script to compute parallel speciation simulations */
source shell_script/00_create_all_output.sh SHORT|MEDIUM|FULL phreeqc|llnl|minteq|wateq4f|sit|ALL

/* Script to fill database having already .pqi, .met and .pqo files from previous script */
source shell_script/00_fill_all_db.sh SHORT|MEDIUM|FULL phreeqc|llnl|minteq|wateq4f|sit|ALL

/* Script to generate all database, from all datasets */
source shell_script/00_compute_and_fill.sh [no argument]



01_LL.sh:

02_cs2cs.sh

03_ehph.sh


SQL scripts ...
===============
LL.sql

cs2cs.sql

VARIO.sql

ehph.sql

