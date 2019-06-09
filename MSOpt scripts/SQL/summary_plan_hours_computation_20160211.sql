-- ==========================================================================================================================================
-- resource: nss
-- date modified: 2016-02-11
-- description:
-- hrs_no_correction:
-- @ no datetime correction, and compute for the hours
-- hrs_correction:
-- @ there is an error on datetime completion and start time in the beginning value, correct by replace and substr function
-- ==========================================================================================================================================


select hr.Machine_Instance,hr.package,case when hr.tooling_family_name='' then ini.Initial_Tool_Family else hr.tooling_family_name end as tooling_family_name , 
case when trim(hr.Initial_Lot_Flag)='Y' then  
	ini.hours 
else 
sum(time_to_sec(timediff(str_to_date(replace(trim(hr.Completion_Time),'00/','12/'),'%m/%d/%Y %H:%i:%s' ) ,str_to_date( 
	(case when substr(hr.Start_Time,1,2)='0/' then concat('12/',substr(hr.start_time,3,18)) end),'%m/%d/%Y %H:%i:%s') ))/3600) end as hours ,
hr.Initial_Lot_Flag,
hr.Completion_Time,
hr.Start_Time,
sum(time_to_sec(timediff(str_to_date(hr.Completion_Time,'%m/%d/%Y %H:%i:%s' ) ,str_to_date( hr.Start_Time,'%m/%d/%Y %H:%i:%s') ))/3600) as hrs_no_correction,
sum(time_to_sec(timediff(str_to_date(replace(trim(hr.Completion_Time),'00/','12/'),'%m/%d/%Y %H:%i:%s' ) ,str_to_date( 
	(case when substr(hr.Start_Time,1,2)='0/' then concat('12/',substr(hr.start_time,3,18)) end),'%m/%d/%Y %H:%i:%s') ))/3600) as hrs_correction
from solution hr 
left join ( select wip.device,wip.cur_qty/r.pph as hours,i.machine_instance,i.initial_tool_family from wip  
left join initialsetup i on wip.Machine_Instance=i.Machine_Instance 
left join route r on r.Machine_Family=i.Machine_Family and r.tooling_family=i.initial_tool_family and wip.device=r.device 
where start_time<>'') ini on ini.device=hr.device_name and ini.machine_instance=hr.Machine_Instance 
group by hr.Machine_Instance, hr.package, hr.tooling_family_name 
;