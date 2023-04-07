#!/bin/bash
# Eqn Eh units = V but DB.Eh Eh[mV]
# F=23100
# R=1.98
# pe=(23100*Eh[V]/(100*(2.303*1.98*T[K])))
# pe=(23100*$8/(1000*(2.303*1.98*($4+273.15))))

ARGS=""

for var in "$@"
do
    ARGS="$ARGS $var"
done

echo ""
echo "Running $BASH_SOURCE $ARGS"

dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

ffile=$1

export pfile=dataset.csv

# Gas constant
R=1.98

# Faraday constant
F=23100

# 
# ID;County;Site_Name;Easting;Northing;Depth;New_Code;Sample_Date;pH_onsite; \\
#  1   2        3       4        5        6      7           8       9

# T;O(0);Eh;Alkalinity;N(-3);N(3);N(5);P;C(-4);Si;Cl;F; S(6);\\
# 10 11  12      13      14  15    16 17  18   19 20 21 22  

# Na;K; Mg;Ca;Fe;Mn;B; Al; Cr; Ni;Cu;Zn;As;Cd;Sb;Ba;Pb;U; Hg;Co;Mo;Sr;Ag;Be
# 23 24 25 26 27 28 28 30  31  32 33 34 35 36 37 38 39 40 41 42 43 44 45 46

# NOTE: N e P
#echo ">>>>>>>>>>>>>>  QUI 1  <<<<<<<<<<<<<<<<<"

awk  -v f=$F -v r=$R -v c=$C 'BEGIN {FS = OFS =";"} {print $1,$2,$3,$4,$5,$6,$7,$8,\
$9,$10,$11,(f*$12/(1000*(2.303*r*($6+273.15)))),\
            $13,$14,$15,$16,$17,$18,$19,\
$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,\
$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,\
$40,$41,$42,$43,$44,$45,$46}' $dir/../scratch/${ffile} > $dir/../scratch/${pfile}
#temp_dataset.txt

echo $dir/../scratch/${ffile}
exit


#echo ">>>>>>>>>>>>>>  QUI 2  <<<<<<<<<<<<<<<<<"

#cp temp_dataset.txt ${pfile}
#
#sed -e 's/;;/;0;/g' -e 's/;;/;0;/g' temp_dataset.txt  > ${pfile}

#echo ">>>>>>>>>>>>>>  QUI 3  <<<<<<<<<<<<<<<<<"

awk  'BEGIN {FS = ";"} {print $1}' $dir/../scratch/${pfile}  | sed '1 d' > $dir/../scratch/samples.dat

#echo "OUT do_preproc"
