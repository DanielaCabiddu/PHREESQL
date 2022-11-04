README

Pre-requisite: SQLite 3.*, PHREEQC, iPHREEQC must be installed

Bash shell scripts should be run inside that folder, otherwise scripts should be changed consistently. The source code PHREESQLIB must be at the same level of that folder.

AVAILABLE SCRIPTS
=================
source shell_script/00_clean_all.sh

source shell_script/00_prepare_folders.sh 

source shell_script/000_gigantic_run.sh EPA_GW_IE_short.csv SHORT EPA_ minteq.dat

source shell_script/00_create_all_output.sh SHORT

source shell_script/00_fill_all_db.sh SHORT

source shell_script/00compute_and_fill.sh



01_LL.sh:

02_cs2cs.sh

03_ehph.sh


SQL scripts ...
===============
LL.sql

cs2cs.sql

VARIO.sql

ehph.sql

