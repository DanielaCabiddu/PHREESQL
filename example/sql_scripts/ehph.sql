

/*${script_folder}/../DB/FULL_wateq4f.db*/

DROP VIEW if exists pe;
DROP VIEW if exists ph;
DROP VIEW if exists tc;
DROP TABLE if exists eh_ph;

create view tc ( ID, COUNTY, SAMPLE, T ) as 
select dos.ANALYSIS_ID, m.SURVEY, m.SAMPLE_NAME, dos.VALUE 
from main.DESCRIPTION_OF_SOLUTION dos
	INNER JOIN main.METADATA m
	ON m.ID = dos.ANALYSIS_ID
WHERE ( dos.PARAM LIKE '%Temperature%' AND m.SURVEY LIKE 'Kilkenny%' ) ;

create view pe as 
select dos.ANALYSIS_ID eh_ID, dos.VALUE pe
from main.DESCRIPTION_OF_SOLUTION dos
WHERE PARAM = 'pe' ;

create view ph as 
select dos.ANALYSIS_ID ph_ID, dos.VALUE ph
from main.DESCRIPTION_OF_SOLUTION dos
WHERE PARAM = 'pH' ;

CREATE TABLE eh_ph as SELECT *
FROM pe as e
INNER JOIN
ph as h
ON e.eh_ID = h.ph_ID
INNER JOIN
tc as t
ON h.ph_ID = t.ID;

ALTER TABLE eh_ph
  ADD eh_V DOUBLE ; 

/* Compute Eh prom pe */
-- R=1.9872
-- F=23100

UPDATE eh_ph SET eh_V = (pe*(2.303*1.9872*(273.15+T))/23100);

/* Delete useless column */
ALTER TABLE eh_ph 
	DROP COLUMN pe;
ALTER TABLE eh_ph 
	DROP COLUMN ph_ID;
ALTER TABLE eh_ph 
	DROP COLUMN ID;
ALTER TABLE eh_ph 
	DROP COLUMN T;

/* Delete records where Eh is exactly 0 == no data */
DELETE FROM eh_ph
WHERE eh_V = 0;
	

/* EXPORT data to file */
.mode csv
.output scratch/eh_ph.csv
.separator ","

select * from eh_ph;

