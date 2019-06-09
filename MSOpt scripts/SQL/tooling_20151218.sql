
(select 'tooling_family','Tooling_Instance', 'tooling_family_name','certification')  
 union (  
 ;
 
select tool.tooling_family,tool.tooling_instance,tool.Tooling_Family_Name,
 if (cs.Initial_Certification is null, 
		if(m.Certification is null,tool.Certification,m.Certification),
			if(cs.Initial_Certification is null,tool.Certification,cs.Initial_Certification)) as initial_certification  
  from tooling tool    
   left join (   
   	select aaa.*,t.certification, case when t.certification<>initial_certification then 1 else 0 end as missmatch from (   
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
  )aaa left join tooling t on t.tooling_Family=aaa.initial_tooling_family  
   ) cs on cs.initial_tooling_family=tool.tooling_family   
   left join machines m on m.Machine_Instance=cs.Machine_Instance  
   left join route r on r.machine_family = cs.machine_family and r.tooling_family = t.tooling_family 
   group by 1,2,3,4
   ;
   
   left join route r on r.machine_family = cs.machine_family and r.tooling_family = t.tooling_family 
   )
   ;
   
   tool_a left join
   (select r.machine_family, r.tooling_family, r.certification from route r) r on r.machine_family, and r.tooling_family = a.tooling_family
   ;
   
   select * from initialsetup;
   
   
   select * from route;
   
  INTO OUTFILE 'C:/optimizer/data/tooling.csv'   
  FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY ''  
  LINES TERMINATED BY '\n'  
  )