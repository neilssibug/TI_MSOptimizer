

select * from initialsetup;
select * from tooling;
select * from route;

-- tooling_family, device, package, certificate

select * from initialsetup i
left join tooling t on t.tooling_family = i.initial_tool_family
;

select i_t.machine_family, i_t.machine_instance, i_t.initial_tool_family, i_t.initial_certification, i_t.tool_certification, r.certification as route_certification
from (select i.machine_family, i.machine_instance, i.initial_tool_family, i.initial_certification, t.certification as tool_certification
from initialsetup i inner join tooling t on t.tooling_family = i.initial_tool_family where Initial_Certification <> '') i_t
inner join route r on r.machine_family = i_t.machine_family and r.tooling_family = i_t.initial_tool_family
group by 1,2,3
having case when (i_t.initial_certification = i_t.tool_certification and  i_t.initial_certification = r.certification) then 'YES' else 'NO' end = 'NO'
;