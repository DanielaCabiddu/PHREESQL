

.open DB/FULL_wateq4f.db


SELECT
(SELECT m.SITE_NAME from METADATA), m.ID, m.DATE, m.COORD_X, m.COORD_Y, sc.MOLALITY*1000*40.01 from METADATA m
INNER JOIN SOLUTION_COMPOSITION sc
	   ON sc.ANALYSIS_ID = m.ID
WHERE ( DATE between '2012/06/01' AND '2012/06/31' AND sc.ELEMENTS = "Ca");

.mode csv
.output VARIO.csv
.separator ";"
