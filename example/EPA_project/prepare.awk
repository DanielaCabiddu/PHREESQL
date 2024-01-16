#!/usr/bin/awk

BEGIN {
  FS=";";
  template=tmpl;
  output=out;
  getline;
  NR=0;
}

{
        # Read relevant fields from input file from GdV
ID=$1
County=$2
Site_Name=$3
Easting=$4
Northing=$5
Depth=$6
New_Code=$7
Sample_Date=$8
pH=$9
T=$10
O0=$11
pe=$12
Alkalinity=$13
N_3=$14
N3=$15
N5=$16
P=$17
C_4=$18
Si=$19
Cl=$20
F=$21
S6=$22
Na=$23
K=$24
Mg=$25
Ca=$26
Fe=$27
Mn=$28
B=$29
Al=$30
Cr=$31
Ni=$32
Cu=$33
Zn=$34
As=$35
Cd=$36
Sb=$37
Ba=$38
Pb=$39
U=$40
Hg=$41
Co=$42
Mo=$43
Sr=$44
Ag=$45
Be=$46


        # Set output filename
        outfile=(output ID ".pqi");

        # Read a line from template, replace special fields, and
        # print result to output file
        while ( (getline ln < template) > 0 )
        {
sub(/{ID}/,ID,ln);
sub(/{County}/,County,ln);
sub(/{Site_Name}/,Site_Name,ln);
sub(/{Easting}/,Easting,ln);
sub(/{Northing}/,Northing,ln);
sub(/{Depth}/,Depth,ln);
sub(/{New_Code}/,New_Code,ln);
sub(/{Sample_Date}/,Sample_Date,ln);
sub(/{pH}/,pH,ln);
sub(/{T}/,T,ln);
sub(/{O0}/,O0,ln);
sub(/{pe}/,pe,ln);
sub(/{Alkalinity}/,Alkalinity,ln);
sub(/{N_3}/,N_3,ln);
sub(/{N3}/,N3,ln);
sub(/{N5}/,N5,ln);
sub(/{P}/,P,ln);
sub(/{C_4}/,C_4,ln);
sub(/{Si}/,Si,ln);
sub(/{Cl}/,Cl,ln);
sub(/{F}/,F,ln);
sub(/{S6}/,S6,ln);
sub(/{Na}/,Na,ln);
sub(/{K}/,K,ln);
sub(/{Mg}/,Mg,ln);
sub(/{Ca}/,Ca,ln);
sub(/{Fe}/,Fe,ln);
sub(/{Mn}/,Mn,ln);
sub(/{B}/,B,ln);
sub(/{Al}/,Al,ln);
sub(/{Cr}/,Cr,ln);
sub(/{Ni}/,Ni,ln);
sub(/{Cu}/,Cu,ln);
sub(/{Zn}/,Zn,ln);
sub(/{As}/,As,ln);
sub(/{Cd}/,Cd,ln);
sub(/{Sb}/,Sb,ln);
sub(/{Ba}/,Ba,ln);
sub(/{Pb}/,Pb,ln);
sub(/{U}/,U,ln);
sub(/{Hg}/,Hg,ln);
sub(/{Co}/,Co,ln);
sub(/{Mo}/,Mo,ln);
sub(/{Sr}/,Sr,ln);
sub(/{Ag}/,Ag,ln);
sub(/{Be}/,Be,ln);
                
                print(ln) > outfile;

        }

        # Close template and output file in advance of next record
        close(outfile);
        close(template);
}


