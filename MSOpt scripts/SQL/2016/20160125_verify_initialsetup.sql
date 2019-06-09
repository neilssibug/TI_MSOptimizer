use optimizer;

select coalesce(m.machine_family,'') as machine_family,aa.Machine_Instance,coalesce(m.Machine_Family_Name,'') as machine_family_name,aa.initial_tooling_family,case when aa.initial_tooling_family='' then '' else coalesce(m.Certification,'') end as Initial_Certification from (  
select coalesce(trim(s.atss_testec_config),'') as machine_family,  
coalesce(trim(case when substr(t.tester_TWSTATUS_i,7,1)=0 then concat('CLFTEA',substr(t.tester_TWSTATUS_i,8,1)) else  
t.tester_TWSTATUS_i end ),'') as machine_instance,coalesce(trim(s.atss_testec_config),'') as machine_family_name,coalesce(trim(s.atss_loadboard),'') as initial_tooling_family,  
case when s.atss_temp>85 then 3  when s.atss_temp >30 and s.atss_temp <=85 then 2 else 1 end as initial_certification from (  
select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from (   
select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i, max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i  from twstatus t where  
substr(t.tester_TWSTATUS_i,1,4)='TT55'   
group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc) aa 
group by 2) t  
left join smsdw s on trim(s.lot)= trim(t.lot_TWSTATUS_i) 
group by 2 
) aa left join machines m on m.Machine_Instance= aa.machine_instance 
where m.machine_family = 'ETS-2-64' and aa.Machine_Instance in ('CLFTEA50','CLFTEA67') 
;

CLFTEA50
CLFTEA67
;

-- twstatus
select * 
from (
select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i, max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i  
from twstatus t where  
substr(t.tester_TWSTATUS_i,1,4)='TT55'   
group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc
) a where tester_TWSTATUS_i in ('CLFTEA50','CLFTEA67')
;


-- twstatus max
select *
from (
select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from (   
select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i, max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i  from twstatus t where  
substr(t.tester_TWSTATUS_i,1,4)='TT55'   
group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc) aa 
group by 2
) a where tester_TWSTATUS_i in ('CLFTEA50','CLFTEA67')
;

-- twstatus and smsdw
select *
from (
select coalesce(trim(s.atss_testec_config),'') as machine_family,  
coalesce(trim(case when substr(t.tester_TWSTATUS_i,7,1)=0 then concat('CLFTEA',substr(t.tester_TWSTATUS_i,8,1)) else  
t.tester_TWSTATUS_i end ),'') as machine_instance,coalesce(trim(s.atss_testec_config),'') as machine_family_name,coalesce(trim(s.atss_loadboard),'') as initial_tooling_family,  
case when s.atss_temp>85 then 3  when s.atss_temp >30 and s.atss_temp <=85 then 2 else 1 end as initial_certification from (  
select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from (   
select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i, max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i  from twstatus t where  
substr(t.tester_TWSTATUS_i,1,4)='TT55'   
group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc) aa 
group by 2) t  
left join smsdw s on trim(s.lot)= trim(t.lot_TWSTATUS_i) 
group by 2 
) a where machine_instance in ('CLFTEA50','CLFTEA67')
;


select * from smsdw
where atss_testec_config = 'ETS-2-64'
and lot in ('6021005','6024418');