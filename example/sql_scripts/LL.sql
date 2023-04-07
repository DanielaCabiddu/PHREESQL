--
--
--
--
--
/* OPEN DATABASE */
/*.open DB/FULL_wateq4f.db*/

/* CLEAN VIEWS and TABLES */
DROP VIEW if exists S;
DROP VIEW if exists Na;
DROP VIEW if exists Cl;
DROP VIEW if exists Ca;
DROP VIEW if exists Mg;
DROP VIEW if exists K;
DROP VIEW if exists Alk;
DROP VIEW if exists LL_2;
DROP VIEW if exists LL_3;
DROP VIEW if exists LL_4;
DROP VIEW if exists LL_5;

DROP TABLE if exists LL;


/* Create VIEWS */
CREATE VIEW S (ID_s, SO4) as 
SELECT m.ID, s.MOLALITY
FROM SOLUTION_COMPOSITION s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( s.ELEMENTS='S(6)' );

CREATE VIEW Na (ID_n, Na) as
SELECT m.ID, s.MOLALITY
FROM SOLUTION_COMPOSITION s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( s.ELEMENTS='Na' );

CREATE VIEW Cl (ID_cl, Cl) as
SELECT m.ID, s.MOLALITY
FROM SOLUTION_COMPOSITION s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( s.ELEMENTS='Cl' );

CREATE VIEW Ca (ID_c, Ca) as
SELECT m.ID, s.MOLALITY
FROM SOLUTION_COMPOSITION s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( s.ELEMENTS='Ca' );

CREATE VIEW Mg (ID_m, Mg) as
SELECT m.ID, s.MOLALITY
FROM SOLUTION_COMPOSITION s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( s.ELEMENTS='Mg' );

CREATE VIEW K (ID_k, K) as
SELECT m.ID, s.MOLALITY
FROM SOLUTION_COMPOSITION s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( s.ELEMENTS='K' );

CREATE VIEW Alk (ID, COUNTY, SAMPLE, DATE, Alk) as
SELECT m.ID, m.SURVEY, m.SAMPLE_NAME, m.DATE, s.MOLALITY
FROM SOLUTION_COMPOSITION s
	INNER JOIN METADATA m
	ON m.ID = s.ANALYSIS_ID
WHERE ( s.ELEMENTS='C(4)' );

/* Create a TABLE */
DROP TABLE if exists LL;

CREATE TABLE LL as SELECT *
FROM Alk as a
INNER JOIN
Ca as c
ON a.ID = c.ID_c
INNER JOIN
Mg as m
ON c.ID_c = m.ID_m
INNER JOIN 
K as k
ON m.ID_m = k.ID_k
INNER JOIN
S as s
ON k.ID_k = s.ID_s
INNER JOIN
Na as n
ON s.ID_s = n.ID_n
INNER JOIN
Cl as cl
ON n.ID_n = cl.ID_cl
;


/* DROP VIEWS */
DROP VIEW S;
DROP VIEW Na;
DROP VIEW Cl;
DROP VIEW Ca;
DROP VIEW Mg;
DROP VIEW K;
DROP VIEW Alk;

/* DROP COLUMNS */
ALTER TABLE LL 
	DROP COLUMN ID_c;
ALTER TABLE LL 
	DROP COLUMN ID_n;
ALTER TABLE LL 
	DROP COLUMN ID_cl;
ALTER TABLE LL 
	DROP COLUMN ID_m;
ALTER TABLE LL 
	DROP COLUMN ID_k;
ALTER TABLE LL 
	DROP COLUMN ID_s;


/* CONVERT mol/l TO meq/l */
CREATE VIEW LL_2 
(ID, County, Sample, Date, Alke, Cae, Mge, Ke, SO4e, Nae, Cle, An, Cat, EB)
as SELECT 
ID, County, Sample, Date, Alk*(-1)*1000, Ca*(2)*1000, Mg*(2)*1000, K*(1)*1000, SO4*(-2)*1000, Na*(1)*1000, Cl*(-1)*1000,
abs(Alk+SO4*(2)+Cl)*1000, abs(Ca*(2)+Mg*(2)+Na+K)*1000, 
((((Ca*(2)+Mg*(2)+Na+K)-abs(Alk+SO4*(2)+Cl))/((Ca*(2)+Mg*(2)+Na+K)+abs(Alk+SO4*(2)+Cl))))*100
from LL;

.mode csv
.output LL.csv
.separator ";"

SELECT * from LL_2;

/* CONVERT in meq/l TO % meq/l  limited to a single COUNTY */

--
-- WHERE County = 'Kilkenny'
--
CREATE VIEW LL_3 
(ID, County, Sample, Date, CaMgeqp, NaKeqp, HCO3eqp, SO4Cleqp)
AS SELECT 
ID, County, Sample, Date, (Cae+Mge)/Cat*50, (Nae+Ke)/Cat*50, abs(Alke/An*50), abs((SO4e+Cle)/An*50)
FROM LL_2
WHERE County = 'Kilkenny';

/* CREATE data files to be used in diagram*/
.mode csv
.output LL_Kilkenny.csv
.separator ";"

SELECT * from LL_3;


--
-- WHERE County = '??'
--
CREATE VIEW LL_4 
(ID, County, Sample, Date, CaMgeqp, NaKeqp, HCO3eqp, SO4Cleqp)
AS SELECT 
ID, County, Sample, Date, (Cae+Mge)/Cat*50, (Nae+Ke)/Cat*50, abs(Alke/An*50), abs((SO4e+Cle)/An*50)
FROM LL_2
WHERE County = 'Donegal';

/* CREATE data files to be used in diagram*/
.mode csv
.output LL_Donegal.csv
.separator ";"

SELECT * from LL_4;


CREATE VIEW LL_5 
(ID, County, Sample, Date, CaMgeqp, NaKeqp, HCO3eqp, SO4Cleqp)
AS SELECT 
ID, County, Sample, Date, (Cae+Mge)/Cat*50, (Nae+Ke)/Cat*50, abs(Alke/An*50), abs((SO4e+Cle)/An*50)
FROM LL_2;

/* CREATE data files to be used in diagram*/
.mode csv
.output LL_ALL.csv
.separator ";"

SELECT * from LL_5;

/* END */
