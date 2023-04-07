#!/bin/bash

# USAGE ./do_prepare.sh EPA_ ${db}.dat ${onfile}
#                        job    db      datafile

# https://opensource.com/article/19/10/advanced-awk

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#echo " start do prepare"

#echo ">>>>>>>>>>>>>>  QUI 4  $dataset <<<<<<<<<<<<<<<<<"

if [[ "$OSTYPE" == "darwin"* ]]; then
	SED="gsed -i "
else
	SED="sed -i "
fi


# create METADATA
awk -v dout=OUT/${db} -v tmpl=$dir/../shell_script/metadata.cfg -v out=$dir/../scratch/${job} -v db=${db} -f $dir/../shell_script/metadata.awk  < $dir/../scratch/${pfile}
find $dir/../scratch/ -name '*.met' -exec mv {} $dir/../run_DB/$dataset/META/${db} \;

#mv $dir/../scratch/*.met $dir/../run_DB/$dataset/META/${db}/

# create PHREEQC input files
awk -v dout=OUT/${db} -v tmpl=$dir/../shell_script/template.pqi -v out=$dir/../scratch/${job} -f $dir/../shell_script/prepare.awk  < $dir/../scratch/${pfile}
find $dir/../scratch/ -name '*.pqi' -exec mv {} $dir/../run_DB/$dataset/IN/${db} 
#mv $dir/../scratch/${job}*.pqi $dir/../run_DB/$dataset/IN/${db}/

find $dir/../run_DB/$dataset/IN/${db} -type f -exec grep -l 'Alkalinity nd' {} + > $dir/../scratch/no_carb.dat
find $dir/../run_DB/$dataset/IN/${db} -type f -exec grep -l 'temp nd' {} + > $dir/../scratch/no_temp.dat
find $dir/../run_DB/$dataset/IN/${db} -type f -exec grep -l 'pH nd' {} + > $dir/../scratch/no_ph.dat
find $dir/../run_DB/$dataset/IN/${db} -type f -exec grep -l 'pe 0$' {} + > $dir/../scratch/no_pe.dat

exit
#grep -rl "Alkalinity nd" $dir/../run_DB/$dataset/IN/${db} > $dir/../scratch/no_carb.dat
#grep -rl "temp nd" $dir/../run_DB/$dataset/IN/${db} > $dir/../scratch/no_temp.dat
#grep -rl "pH nd" $dir/../run_DB/$dataset/IN/${db} > $dir/../scratch/no_ph.dat
#grep -rl "pe 0$" $dir/../run_DB/$dataset/IN/${db} > $dir/../scratch/no_pe.dat

#echo ">>>>>>>>>>>>>>  QUI 5  e db "${db}" <<<<<<<<<<<<<<<<<"

awk -F ".pqi" '{print substr($1,0,length($1))}' $dir/../scratch/no_carb.dat > $dir/../scratch/temp_
awk -F "${db}/${job}" '{print substr($2,0,length($2))}' $dir/../scratch/temp_ > $dir/../scratch/temp

for i in `cat $dir/../scratch/temp`
do
  $SED "/${i}/d" $dir/../scratch/samples.dat
  #rm -f $dir/../run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  #rm -f $dir/../run_DB/$dataset/META/${db}/${job}${i}.met
done

#echo ">>>>>>>>>>>>>>  QUI 6  <<<<<<<<<<<<<<<<<"

awk -F ".pqi" '{print substr($1,0,length($1))}' $dir/../scratch/no_temp.dat > $dir/../scratch/temp_
awk -F "${db}/${job}" '{print substr($2,0,length($2))}' $dir/../scratch/temp_ > $dir/../scratch/temp
for i in `cat $dir/../scratch/temp`
do
  $SED "/${i}/d" $dir/../scratch/samples.dat
  #rm -f $dir/../run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  #rm -f $dir/../run_DB/$dataset/META/${db}/${job}${i}.met
done

#echo ">>>>>>>>>>>>>>  QUI 7  <<<<<<<<<<<<<<<<<"

awk -F ".pqi" '{print substr($1,0,length($1))}' $dir/../scratch/no_ph.dat > $dir/../scratch/temp_
awk -F "${db}/${job}" '{print substr($2,0,length($2))}' $dir/../scratch/temp_ > $dir/../scratch/temp
for i in `cat $dir/../scratch/temp`
do
  $SED "/${i}/d" $dir/../scratch/samples.dat
  #rm -f $dir/../run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  #rm -f $dir/../run_DB/$dataset/META/${db}/${job}${i}.met
done

#echo ">>>>>>>>>>>>>>  QUI 8  <<<<<<<<<<<<<<<<<"

awk -F ".pqi" '{print substr($1,0,length($1))}' $dir/../scratch/no_pe.dat > $dir/../scratch/temp_
awk -F "${db}/${job}" '{print substr($2,0,length($2))}' $dir/../scratch/temp_ > $dir/../scratch/temp
for i in `cat $dir/../scratch/temp`
do
FILE=$dir/../run_DB/$dataset/IN/${db}/${job}${i}.pqi
if [ -f "$FILE" ]; then
  $SED "s/^pe 0$/pe nd/g" $FILE
fi
done

#echo ">>>>>>>>>>>>>>  QUI 9  <<<<<<<<<<<<<<<<<"

for i in `cat $dir/../scratch/samples.dat`
do
FILE=$dir/../run_DB/$dataset/IN/${db}/${job}${i}.pqi
if [ -f "$FILE" ]; then
  $SED '/nd/d' $FILE
fi
done

#echo "exit do_prepare"

