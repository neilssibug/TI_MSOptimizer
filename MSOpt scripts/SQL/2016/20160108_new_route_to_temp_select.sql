

truncate route_temp;

insert into route_temp
select rf1.* from route rf1
left join 
(select rt.* , r.setup_count,r2.temp_count,r2.certification as temp_count_cert
from (select rt.*,i.Machine_Instance from route rt
inner join initialsetup i  on rt.machine_family = i.machine_family and rt.tooling_family = i.initial_tool_family) rt
left join 
(select r.*, count(*) as setup_count -- setup count with machine,tooling,logpoit and device
from route r
where trim(logpoint) <> '4420' and alternate=''
group by r.logpoint,r.device, r.machine_family, r.tooling_family ) r on r.logpoint=rt.logpoint and r.device=rt.device and r.machine_family = rt.machine_family and r.tooling_family = rt.tooling_family
left join 
(select max(temp_count) as temp_count,rf.Certification,rf.logpoint,rf.device, rf.machine_family, rf.tooling_family from (
select r.*, count(*) as temp_count -- number of setup with same cert/temp
from route r
where trim(logpoint) <> '4420' and alternate=''
group by r.logpoint,r.device, r.machine_family, r.tooling_family,r.certification
order by id asc
)rf
group by rf.logpoint,rf.device, rf.machine_family, rf.tooling_family) r2 on r2.logpoint=rt.logpoint and r2.device=rt.device and r2.machine_family = rt.machine_family and r2.tooling_family = rt.tooling_family 
where rt.alternate='' and rt.logpoint='4410' and setup_count > 1
) rf2 
on rf2.routename=rf1.routename and rf2.logpoint=rf1.logpoint and rf2.operation=rf1.operation and rf2.lpt_desc=rf1.lpt_desc 
and rf2.opn_desc=rf1.opn_desc and rf2.device=rf1.device and rf2.package=rf1.package and rf2.pin=rf1.pin and rf2.alternate=rf1.alternate 
and rf2.setup=rf1.setup and rf2.pph=rf1.pph and rf2.machine_family=rf1.machine_family and rf2.tooling_family=rf1.tooling_family 
and rf2.toolqty=rf1.toolqty and rf2.certification=rf1.certification
;

select count(*) from route;
rf2.logpoint=rf1.logpoint and rf2.device=rf1.device and rf2.machine_family = rf1.machine_family and rf2.tooling_family = rf1.tooling_family 
;

select * from route_temp;