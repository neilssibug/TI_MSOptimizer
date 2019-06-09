select * from smsdw;

select * from device_cap;

select * from device_cap left join on 


COALESCE((sp_pph.LB),'')
;


select s.id, s.Machine_Instance, s.Machine_Family_Name, s.Package, s.Tooling_Family_Name, s.Quantity,s.device_name, pin    
         from solution s inner join wip on wip.device = s.device_name and s.lot_name = wip.lot_name;
         
         
         select * from (select t.tester_TWSTATUS_i as machine_instance,s.atss_loadboard ,s.atss_testec_config as machine_family,     
 			case when s.atss_temp =25 then 1  when s.atss_temp >25 then 2 when s.atss_temp <25 then 3    end as certification     
 			from (select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from (     
 			select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i,     
 			max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i      
             from twstatus t where substr(t.tester_TWSTATUS_i,1,4)='TT55'     
 			group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc) aa     
 			group by 2) t left join smsdw s  on t.lot_TWSTATUS_i = s.lot) aa where machine_family like 'ET%' group by 1,2   ;
            
            
             select * from (select coalesce(trim(case when substr(t.tester_TWSTATUS_i,7,1)=0 then 
				concat('CLFTEA',substr(t.tester_TWSTATUS_i,8,1)) else t.tester_TWSTATUS_i end ),'') as machine_instance, s.atss_loadboard ,s.atss_testec_config as machine_family,     
 			case when s.atss_temp =25 then 1  when s.atss_temp >25 then 2 when s.atss_temp <25 then 3    end as certification     
 			from (select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from (     
 			select 
            concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i,     
 			max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i      
             from twstatus t where substr(t.tester_TWSTATUS_i,1,4)='TT55'     
 			group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc) aa     
 			group by 2) t left join smsdw s  on t.lot_TWSTATUS_i = s.lot) aa where machine_family like 'ET%' group by 1,2   ;