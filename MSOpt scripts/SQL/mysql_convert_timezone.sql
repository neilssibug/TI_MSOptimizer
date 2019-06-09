
SELECT CONVERT_TZ('2004-01-01 12:00:00','GMT');

select Completion_Time, Start_Time,str_to_date((case when substr(Start_Time,1,2)='0/' then concat('12/',substr(start_time,3,18)) end),'%m/%d/%Y %H:%i:%s') as dallas,
CONVERT_TZ(str_to_date((case when substr(Start_Time,1,2)='0/' then concat('12/',substr(start_time,3,18)) end),'%m/%d/%Y %H:%i:%s'),'-06:00','+08:00') as start_time
from solution
;


select * from solution where machin_instance = 'CL';