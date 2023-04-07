/* OPEN DATABASE */
.open DB/FULL_wateq4f.db

drop
	view if exists calcite;
drop
	view if exists pCO2;
drop
	table if exists SAT; 
drop 
	view if exists Kilkenny;
drop 
	view if exists Donegal;


CREATE view calcite (
		ID_c, IFI_c, D_c, SN_c, S_c, pH_c, sicc_c
	) as
SELECT
	m.ID,
	m.INPUT_FILE,
	m.DATE,
	m.SAMPLE_NAME,
	m.SURVEY,
	dos.VALUE pH,
	si.SI SI_cc
FROM
	METADATA m
	INNER JOIN DESCRIPTION_OF_SOLUTION dos ON m.ID = dos.ANALYSIS_ID
	INNER JOIN SATURATION_INDICES si ON dos.ANALYSIS_ID = si.ANALYSIS_ID
WHERE
	si.PHASE = 'Calcite'
	and abs(SI_cc) < 5
	and (
		pH > 6.35
		and pH < 10.33
	) 
; 

CREATE view pCO2 (
		ID_p, IFI_p, D_p, SN_p, S_p, pH_p, sico2_p
	) as
SELECT
	m.ID,
	m.INPUT_FILE,
	m.DATE,
	m.SAMPLE_NAME,
	m.SURVEY,
	dos.VALUE pH,
	si.SI pCO2
FROM
	METADATA m
	INNER JOIN DESCRIPTION_OF_SOLUTION dos ON m.ID = dos.ANALYSIS_ID
	INNER JOIN SATURATION_INDICES si ON dos.ANALYSIS_ID = si.ANALYSIS_ID
WHERE
	si.PHASE = 'CO2(g)'
	and abs(pCO2) < 5
	and (
		pH > 6.35
		and pH < 10.33
	)
;


CREATE TABLE SAT  as 
SELECT *
FROM calcite as c
INNER JOIN pCO2 as p ON c.ID_c = p.ID_p;

ALTER TABLE SAT 
	DROP COLUMN ID_p;
ALTER TABLE SAT 
	DROP COLUMN IFI_p;
ALTER TABLE SAT 
	DROP COLUMN D_p;
ALTER TABLE SAT 
	DROP COLUMN SN_p;
ALTER TABLE SAT 
	DROP COLUMN S_p;
ALTER TABLE SAT 
	DROP COLUMN pH_p;

CREATE view Donegal as
SELECT
*
FROM
SAT s
WHERE
S_c = 'Donegal'
;

.mode csv
.output scratch/SI_Donegal.csv
.separator ";"

SELECT * from Donegal;

CREATE view Kilkenny as
SELECT
*
FROM
SAT s
WHERE
S_c = 'Kilkenny'
;

.mode csv
.output scratch/SI_Kilkenny.csv
.separator ";"

SELECT * from Kilkenny;


