#!/usr/bin/awk

BEGIN {
  FS=";";
  template=tmpl;
  output=out;
  getline;
  NR=0;
  db=db;
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
#
#
Database=db
Version="3.6.1-15000"
Epsg="29902"
Input=out ID".pqi"
Survey="EPA database from EPA-IE website"
Run="00000"
Stamp="000000000000"





        # Set output filename
        outfile=(output ID ".met");

        # Read a line from template, replace special fields, and
        # print result to output file
        while ( (getline ln < template) > 0 )
        {
sub(/{ID}/,ID,ln);
sub(/{JOB}/,Survey,ln);
sub(/{SURVEY}/,County,ln);
sub(/{SITE_NAME}/,Site_Name,ln);
sub(/{EASTING}/,Easting,ln);
sub(/{NORTHING}/,Northing,ln);
sub(/{DEPTH}/,Depth,ln);
sub(/{DATE}/,Sample_Date,ln);
sub(/{DB}/,Database,ln);
sub(/{VERSION}/,Version,ln);
sub(/{EPSG}/,Epsg,ln);
sub(/{SAMPLE}/,New_Code,ln);
sub(/{INPUT}/,Input,ln);
sub(/{RUN}/,Run,ln);
sub(/{STAMP}/,Stamp,ln);
                print(ln) > outfile;

        }
        # Close template and output file in advance of next record
        close(outfile);
        close(template);
}


