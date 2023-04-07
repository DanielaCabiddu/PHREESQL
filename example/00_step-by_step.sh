#! /bin/bash

time source shell_script/00_clean_all.sh 
time source shell_script/00_prepare_folders.sh 

################################################################

time source shell_script/000_gigantic_run.sh EPA_GW_IE_short.csv SHORT EPA_ phreeqc.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE_short.csv SHORT EPA_ llnl.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE_short.csv SHORT EPA_ minteq.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE_short.csv SHORT EPA_ wateq4f.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE_short.csv SHORT EPA_ sit.dat

################################################################
time source shell_script/000_gigantic_run.sh EPA_GW_IE_medium.csv MEDIUM EPA_ phreeqc.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE_medium.csv MEDIUM EPA_ llnl.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE_medium.csv MEDIUM EPA_ minteq.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE_medium.csv MEDIUM EPA_ wateq4f.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE_medium.csv MEDIUM EPA_ sit.dat

################################################################
time source shell_script/000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ phreeqc.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ llnl.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ minteq.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ wateq4f.dat
time source shell_script/000_gigantic_run.sh EPA_GW_IE.csv FULL EPA_ sit.dat

################################################################
time source shell_script/00_create_all_output.sh SHORT phreeqc
time source shell_script/00_create_all_output.sh SHORT llnl
time source shell_script/00_create_all_output.sh SHORT minteq
time source shell_script/00_create_all_output.sh SHORT wateq4f
time source shell_script/00_create_all_output.sh SHORT sit

################################################################
time source shell_script/00_create_all_output.sh MEDIUM phreeqc
time source shell_script/00_create_all_output.sh MEDIUM llnl
time source shell_script/00_create_all_output.sh MEDIUM minteq
time source shell_script/00_create_all_output.sh MEDIUM wateq4f
time source shell_script/00_create_all_output.sh MEDIUM sit

################################################################
time source shell_script/00_create_all_output.sh FULL phreeqc
time source shell_script/00_create_all_output.sh FULL llnl
time source shell_script/00_create_all_output.sh FULL minteq
time source shell_script/00_create_all_output.sh FULL wateq4f
time source shell_script/00_create_all_output.sh FULL sit

# SHORT: 42 / MEDIUM: 4354 / FULL: 14690
################################################################
time source shell_script/00_fill_all_db.sh SHORT phreeqc
time source shell_script/00_fill_all_db.sh SHORT llnl
time source shell_script/00_fill_all_db.sh SHORT minteq
time source shell_script/00_fill_all_db.sh SHORT wateq4f
time source shell_script/00_fill_all_db.sh SHORT sit

################################################################
time source shell_script/00_fill_all_db.sh MEDIUM phreeqc
time source shell_script/00_fill_all_db.sh MEDIUM llnl
time source shell_script/00_fill_all_db.sh MEDIUM minteq
time source shell_script/00_fill_all_db.sh MEDIUM wateq4f
time source shell_script/00_fill_all_db.sh MEDIUM sit

################################################################ (14688)
time source shell_script/00_fill_all_db.sh FULL phreeqc
time source shell_script/00_fill_all_db.sh FULL llnl
time source shell_script/00_fill_all_db.sh FULL minteq
time source shell_script/00_fill_all_db.sh FULL wateq4f
time source shell_script/00_fill_all_db.sh FULL sit

################################################################
time source shell_script/00_fill_all_db.sh SHORT ALL
time source shell_script/00_fill_all_db.sh MEDIUM ALL
time source shell_script/00_fill_all_db.sh FULL ALL
