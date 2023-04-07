

.open DB/FULL_ALL.db

DROP view if exists S ;
DROP view if exists Sphreeqc ;
DROP view if exists Sllnl ;
DROP view if exists Sminteq ;
DROP view if exists Ssit ;
DROP view if exists Swateq4f ;

CREATE VIEW Sphreeqc (ID_m, ID_s, sample, dattet, db, chalcedony) as 
SELECT m.ID, s.ANALYSIS_ID, m.SAMPLE_NAME, m.DATE, m.DATABASE, s.SI
FROM SATURATION_INDICES s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( m.SAMPLE_NAME = 'IE_EA_G_0010_2100_0009' AND s.PHASE='Chalcedony' AND m.DATABASE='phreeqc');


.mode csv
.output scratch/SI_chal_phreeqc.csv
.separator ";"

SELECT * from Sphreeqc;


CREATE VIEW Sllnl (ID_m, ID_s, sample, dattet, db, chalcedony) as 
SELECT m.ID, s.ANALYSIS_ID, m.SAMPLE_NAME, m.DATE, m.DATABASE, s.SI
FROM SATURATION_INDICES s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( m.SAMPLE_NAME = 'IE_EA_G_0010_2100_0009' AND s.PHASE='Chalcedony' AND m.DATABASE='llnl');



.mode csv
.output scratch/SI_chal_llnl.csv
.separator ";"

SELECT * from Sllnl;


CREATE VIEW Sminteq (ID_m, ID_s, sample, dattet, db, chalcedony) as 
SELECT m.ID, s.ANALYSIS_ID, m.SAMPLE_NAME, m.DATE, m.DATABASE, s.SI
FROM SATURATION_INDICES s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( m.SAMPLE_NAME = 'IE_EA_G_0010_2100_0009' AND s.PHASE='Chalcedony' AND m.DATABASE='minteq');



.mode csv
.output scratch/SI_chal_minteq.csv
.separator ";"

SELECT * from Sminteq;


CREATE VIEW Ssit (ID_m, ID_s, sample, dattet, db, chalcedony) as 
SELECT m.ID, s.ANALYSIS_ID, m.SAMPLE_NAME, m.DATE, m.DATABASE, s.SI
FROM SATURATION_INDICES s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( m.SAMPLE_NAME = 'IE_EA_G_0010_2100_0009' AND s.PHASE='Chalcedony' AND m.DATABASE='sit');



.mode csv
.output scratch/SI_chal_sit.csv
.separator ";"

SELECT * from Ssit;


CREATE VIEW Swateq4f (ID_m, ID_s, sample, dattet, db, chalcedony) as 
SELECT m.ID, s.ANALYSIS_ID, m.SAMPLE_NAME, m.DATE, m.DATABASE, s.SI
FROM SATURATION_INDICES s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( m.SAMPLE_NAME = 'IE_EA_G_0010_2100_0009' AND s.PHASE='Chalcedony' AND m.DATABASE='wateq4f');



.mode csv
.output scratch/SI_chal_wateq4f.csv
.separator ";"

SELECT * from Swateq4f;