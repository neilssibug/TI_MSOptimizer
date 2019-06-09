 select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,'' as START_TIME,SNAPSHOOT_TIME,'' as MACHINE_INSTANCE from (   
 select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,   
 CUM_PLAN_CT,LOT_AGE,'' as START_TIME,SNAPSHOOT_TIME,'' as MACHINE_INSTANCE,wipdays,ID,   
 case when keydev=1 and key_total>(kd.Target * 1.1 ) and one_dev=0 then 9999 when keydev=1 then 0.1 else round(cumwipdays,2) end as cumwipdays,key_total,tempdev,keydev,target * 1.1 as target    
 from (   
	 select a.LOT,a.DEVICE,a.PACKAGE,a.PIN,a.CUR_QTY,a.WEIGHT,a.LOT_NAME,a.LPT,a.OPN,a.TOTAL_PLAN_CT,a.CUM_PLAN_CT,a.LOT_AGE,'' as START_TIME,a.SNAPSHOOT_TIME,'' as MACHINE_INSTANCE,a.wipdays,a.ID    
"		 ,case when @prev_device = a.device then @cumtotal := @cumtotal + a.wipdays else @cumtotal := a.wipdays end as cumwipdays    
"		 ,case when @prev_device = a.device and a.keydev=1 then @qtytotal := @qtytotal + a.CUR_QTY else @qtytotal := a.CUR_QTY end as key_total    
 		 ,(@prev_device := a.device) as tempdev    
"		 , a.keydev   
"		, case when w1.device=a.device and w1.id=a.id then 1 else 0 end as one_dev   
 	 from (   
		  select @prev_device := null    
		  ,@cumtotal := 0    
		  ,@qtytotal := 0   
	  ) i    
	  join (    
		  select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,    
		  case when left(START_TIME,2)= ' '  then replace(START_TIME,' ' ,'' ) else START_TIME end  as START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,wipdays,ID,keydev 
		  from (    
				  select w2.*,result2.wipdays,keydev from    
				  wip w2 left join    
				  (    
					  select result1.*, result1.initial_cumu as wipdays from     
					  (    
					  select    
					  wip.*,round(wip.cur_qty / (r.pph * 24),2) as initial_cumu    
					  , case when wip.device in (select key_device from keydevices k) then 1 else 0 end as keydev   
					  from wip    
					  LEFT join route r on r.Device=wip.device    
					  ) result1    
					  group by result1.device    
					 order by device,weight desc, id asc   
				  ) result2    
				  on w2.device = result2.device    
			   ) aa    
		   order by device,weight desc ,id asc   
	  ) a left join (select device,min(id) as id from wip w2 group by w2.device) w1 on w1.device=a.device and w1.id=a.id   
 )fr1 left join keydevices kd on fr1.device=kd.key_device   
 order by ID   
 )fn   
 where cumwipdays<>9999   
 order by ID  