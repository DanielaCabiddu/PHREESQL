

/*.open DB/FULL_wateq4f_2157.db*/

create view cs2cs (site, ID, name, date, X, Y, SI, phase) as
select m.SITE_NAME, m.ID, m.SAMPLE_NAME, m.DATE, m.COORD_X, m.COORD_Y, si.SI, si.PHASE from METADATA m 
INNER JOIN SATURATION_INDICES si
	   ON si.ANALYSIS_ID = m.ID
WHERE ( si.PHASE='Calcite' AND (DATE between '2012/07/01' AND '2012/08/31')
AND m.SITE_NAME NOT LIKE '%Deep%' 
AND m.SITE_NAME NOT LIKE '%Transition%' 
AND m.SITE_NAME NOT LIKE '%Subsoil%' 
AND m.SITE_NAME NOT LIKE '%Shallow1%' 
AND m.SITE_NAME NOT LIKE '%Shallow 2%' 
AND m.SITE_NAME NOT LIKE '%Shallow2%' );

SELECT * from cs2cs;

.mode csv
.output cs2cs.csv
.separator ";"
