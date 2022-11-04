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
New_Code=$6
Sample_Date=$7
pH=$8
T=$9
O0=$10
pe=$11
Alkalinity=$12
N_3=$13
N3=$14
N5=$15
P=$16
C_4=$17
Si=$18
Cl=$19
F=$20
S6=$21
Na=$22
K=$23
Mg=$24
Ca=$25
Fe=$26
Mn=$27
B=$28
Al=$29
Cr=$30
Ni=$31
Cu=$32
Zn=$33
As=$34
Cd=$35
Sb=$36
Ba=$37
Pb=$38
U=$39
Hg=$40
Co=$41
Mo=$42
Sr=$43
Ag=$44
Be=$45


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


