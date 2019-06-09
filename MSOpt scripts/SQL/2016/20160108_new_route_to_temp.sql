
select rt.* , r.setup_count,r2.temp_count,r2.certification as temp_count_cert
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
where rt.alternate='' and rt.logpoint='4410'
;
