#!/bin/bash

# USAGE ./do_prepare.sh EPA_ ${db}.dat ${onfile} EPA_project
#                        job    db      datafile    project

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
awk -v dout=OUT/${db} -v tmpl=$dir/../${project}/metadata.cfg -v out=$dir/../${project}/scratch/${job} -v db=${db} -f $dir/../${project}/metadata.awk  < $dir/../${project}/scratch/${pfile}
find $dir/../${project}/scratch/ -name '*.met' -exec mv {} $dir/../${project}/run_DB/$dataset/META/${db} \;

#mv $dir/../scratch/*.met $dir/../run_DB/$dataset/META/${db}/

# create PHREEQC input files
awk -v dout=OUT/${db} -v tmpl=$dir/../${project}/template.pqi -v out=$dir/../${project}/scratch/${job} -f $dir/../${project}/prepare.awk  < $dir/../${project}/scratch/${pfile}
find $dir/../${project}/scratch/ -name '*.pqi' -exec mv {} $dir/../${project}/run_DB/$dataset/IN/${db} \;
#mv $dir/../scratch/${job}*.pqi $dir/../run_DB/$dataset/IN/${db}/

find $dir/../${project}/run_DB/$dataset/IN/${db} -type f -exec grep -l 'Alkalinity nd' {} + > $dir/../${project}/scratch/no_carb.dat
find $dir/../${project}/run_DB/$dataset/IN/${db} -type f -exec grep -l 'temp nd' {} + > $dir/../${project}/scratch/no_temp.dat
find $dir/../${project}/run_DB/$dataset/IN/${db} -type f -exec grep -l 'pH nd' {} + > $dir/../${project}/scratch/no_ph.dat
find $dir/../${project}/run_DB/$dataset/IN/${db} -type f -exec grep -l 'pe 0$' {} + > $dir/../${project}/scratch/no_pe.dat

#echo "Samples: " `cat $dir/../scratch/samples.dat | wc -l`
#echo "No Temp: " `cat $dir/../scratch/no_temp.dat | wc -l`
#echo "No Carb: " `cat $dir/../scratch/no_carb.dat | wc -l`
#echo "No ph: " `cat $dir/../scratch/no_ph.dat | wc -l`
#echo "No pe: " `cat $dir/../scratch/no_pe.dat | wc -l`

#grep -rl "Alkalinity nd" $dir/../run_DB/$dataset/IN/${db} > $dir/../scratch/no_carb.dat
#grep -rl "temp nd" $dir/../run_DB/$dataset/IN/${db} > $dir/../scratch/no_temp.dat
#grep -rl "pH nd" $dir/../run_DB/$dataset/IN/${db} > $dir/../scratch/no_ph.dat
#grep -rl "pe 0$" $dir/../run_DB/$dataset/IN/${db} > $dir/../scratch/no_pe.dat

#echo ">>>>>>>>>>>>>>  QUI 5  e db "${db}" <<<<<<<<<<<<<<<<<"

awk -F ".pqi" '{print substr($1,0,length($1))}' $dir/../${project}/scratch/no_carb.dat > $dir/../${project}/scratch/temp_
awk -F "${db}/${job}" '{print substr($2,0,length($2))}' $dir/../${project}/scratch/temp_ > $dir/../${project}/scratch/temp

for i in `cat $dir/../${project}/scratch/temp`
do
  $SED "/^${i}$/d" $dir/../${project}/scratch/samples.dat
  rm -f $dir/../${project}/run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  rm -f $dir/../${project}/run_DB/$dataset/META/${db}/${job}${i}.met
done

#echo ">>>>>>>>>>>>>>  QUI 6  <<<<<<<<<<<<<<<<<"

awk -F ".pqi" '{print substr($1,0,length($1))}' $dir/../${project}/scratch/no_temp.dat > $dir/../${project}/scratch/temp_
awk -F "${db}/${job}" '{print substr($2,0,length($2))}' $dir/../${project}/scratch/temp_ > $dir/../${project}/scratch/temp
for i in `cat $dir/../${project}/scratch/temp`
do
  $SED "/^${i}$/d" $dir/../${project}/scratch/samples.dat
  rm -f $dir/../${project}/run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  rm -f $dir/../${project}/run_DB/$dataset/META/${db}/${job}${i}.met
done

#echo ">>>>>>>>>>>>>>  QUI 7  <<<<<<<<<<<<<<<<<"

awk -F ".pqi" '{print substr($1,0,length($1))}' $dir/../${project}/scratch/no_ph.dat > $dir/../${project}/scratch/temp_
awk -F "${db}/${job}" '{print substr($2,0,length($2))}' $dir/../${project}/scratch/temp_ > $dir/../${project}/scratch/temp
for i in `cat $dir/../${project}/scratch/temp`
do
  $SED "/^${i}$/d" $dir/../${project}/scratch/samples.dat
  rm -f $dir/../${project}/run_DB/$dataset/IN/${db}/${job}${i}.pqi 
  rm -f $dir/../${project}/run_DB/$dataset/META/${db}/${job}${i}.met
done

#echo ">>>>>>>>>>>>>>  QUI 8  <<<<<<<<<<<<<<<<<"

awk -F ".pqi" '{print substr($1,0,length($1))}' $dir/../${project}/scratch/no_pe.dat > $dir/../${project}/scratch/temp_
awk -F "${db}/${job}" '{print substr($2,0,length($2))}' $dir/../${project}/scratch/temp_ > $dir/../${project}/scratch/temp
for i in `cat $dir/../${project}/scratch/samples.dat`
do
FILE=$dir/../${project}/run_DB/$dataset/IN/${db}/${job}${i}.pqi
if [ -f "$FILE" ]; then
  $SED "s/^pe 0$/pe nd/g" $FILE
fi
done

#echo ">>>>>>>>>>>>>>  QUI 9  <<<<<<<<<<<<<<<<<"

for i in `cat $dir/../${project}/scratch/samples.dat`
do
FILE=$dir/../${project}/run_DB/$dataset/IN/${db}/${job}${i}.pqi
if [ -f "$FILE" ]; then
  $SED '/nd/d' $FILE
fi
done

#echo "exit do_prepare"

