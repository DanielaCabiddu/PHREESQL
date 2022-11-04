#!/bin/bash

# USAGE ./do_prepare.sh EPA_ ${db}.dat ${onfile}
#                        job    db      datafile

# https://opensource.com/article/19/10/advanced-awk

echo " start do prepare"

echo ">>>>>>>>>>>>>>  QUI 4  $dataset <<<<<<<<<<<<<<<<<"


# create METADATA
awk -v dout=OUT/${db} -v tmpl=shell_script/metadata.cfg -v out=${job} -v db=${db} -f shell_script/metadata.awk  < scratch/${pfile}
mv ${job}*.met run_DB/$dataset/META/${db}/



# create PHREEQC input files
awk -v dout=OUT/${db} -v tmpl=shell_script/template.pqi -v out=${job} -f shell_script/prepare.awk  < scratch/${pfile}
mv ${job}*.pqi run_DB/$dataset/IN/${db}/

grep "Alkalinity nd" run_DB/$dataset/IN/${db}/* > scratch/no_carb.dat
grep "temp nd" run_DB/$dataset/IN/${db}/* > scratch/no_temp.dat
grep "pH nd" run_DB/$dataset/IN/${db}/* > scratch/no_ph.dat
grep "pe 0$" run_DB/$dataset/IN/${db}/* > scratch/no_pe.dat

case "${db}" in
"phreeqc")
le=37
;;
"llnl")
le=34
;;
"minteq")
le=36
;;
"wateq4f")
le=37
;;
"sit")
le=33
;;
esac
echo ">>>>>>>>>>>>>>  QUI 5  e db "${db}" <<<<<<<<<<<<<<<<<"

awk -v c="${le}" -F '.' '{print substr($1,c,length($1))}' scratch/no_carb.dat > scratch/temp
for i in `cat scratch/temp`
do
  sed -i '' "/${i}/d" scratch/samples.dat
  rm run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  rm run_DB/$dataset/META/${db}/${job}${i}.met
done
echo ">>>>>>>>>>>>>>  QUI 6  <<<<<<<<<<<<<<<<<"
awk -v c=${le} -F '.' '{print substr($1,c,length($1))}' scratch/no_temp.dat > scratch/temp
for i in `cat scratch/temp`
do
  sed -i '' "/${i}/d" scratch/samples.dat
  rm run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  rm run_DB/$dataset/META/${db}/${job}${i}.met
done
echo ">>>>>>>>>>>>>>  QUI 7  <<<<<<<<<<<<<<<<<"

awk -v c=${le} -F '.' '{print substr($1,c,length($1))}' scratch/no_pH.dat > scratch/temp
for i in `cat scratch/temp`
do
  sed -i '' "/${i}/d" scratch/samples.dat
  rm run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  rm run_DB/$dataset/META/${db}/${job}${i}.met
done
echo ">>>>>>>>>>>>>>  QUI 8  <<<<<<<<<<<<<<<<<"

awk -v c=${le} -F '.' '{print substr($1,c,length($1))}' scratch/no_pe.dat > scratch/temp
for i in `cat scratch/temp`
do
  sed -i '' 's/^pe 0$/pe nd/g' run_DB/$dataset/IN/${db}/${job}${i}.pqi
done

for i in `cat scratch/samples.dat`
do
  sed -i '' ' /nd/d ' run_DB/$dataset/IN/${db}/${job}${i}.pqi
done

echo "exit do_prepare"

