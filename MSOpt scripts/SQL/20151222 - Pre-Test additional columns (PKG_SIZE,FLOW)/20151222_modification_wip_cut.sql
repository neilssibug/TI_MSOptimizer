
use toptimizer;



 (select 'LOT','DEVICE','PACKAGE','PIN','CUR_QTY','WEIGHT','LOT_NAME','LPT','OPN','TOTAL_PLAN_CT','CUM_PLAN_CT','LOT_AGE','START_TIME','SNAPSHOOT_TIME','MACHINE_INSTANCE','PKG_SIZE','FLOW') 
 union 
 ( select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,CASE WHEN LPT<>'4420' THEN '4410' ELSE '4420' END AS LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,PKG_SIZE,FLOW from ( 
 select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,wipdays,ID,cumwipdays,key_total,tempdev,keydev,target,queue,current_lb,pkg_size,flow from ( 
 select LOT,fr1.DEVICE,fr1.PACKAGE,fr1.PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT, 
 CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,wipdays,fr1.ID,current_lb 
 ,case when keydev=1 or current_lb=1 then 9999 else if(fr1.device in (select r.device from route r where r.tooling_family in (select atss_loadboard from smsdw)),9999,round((wip_total.total_wip/round((if(cap.capacity is null or cap.capacity = '',(select avg(capacity) from device_cap),cap.capacity )),2)),2))  end as queue   
 ,case when keydev=1 and key_total>(kd.Target * 1.1 ) and one_dev=0 then 9999 when keydev=1 then 0.1 else round( if(fr1.device in (select r.device from route r where r.tooling_family in (select atss_loadboard from smsdw)),0.1,cumwipdays),2) end as cumwipdays,key_total,tempdev,keydev,target * 1.1 as target   
 ,case when keydev=1 then 0 when wip_total.total_wip<(select value from msoptconfig where trim(configname) = 'DEFAULT_CAPACITY_PER_DAY_PER_TESTER' ) then 1 else 0 end as below_lb_queue
 ,case when keydev=1 then 0 when gpkg.total_wip_pkg<(select value from msoptconfig where trim(configname) = 'DEFAULT_CAPACITY_PER_DAY_PER_TESTER_PKG_SZE') then 1 else 0 end as below_pkg_queue
 ,fr1.pkg_size
 ,fr1.flow
 from (  
 	select a.LOT,a.DEVICE,a.PACKAGE,a.PIN,a.CUR_QTY,a.WEIGHT,a.LOT_NAME,a.LPT,a.OPN,a.TOTAL_PLAN_CT,a.CUM_PLAN_CT,a.LOT_AGE, case when start_time<>'' then date_format(CONVERT_TZ(str_to_date(trim(start_time),'%m/%d/%Y %H:%i:%s'),'-06:00','+08:00'),'%m/%d/%Y %H:%i:%s') else '' end as start_time,a.SNAPSHOOT_TIME, MACHINE_INSTANCE,a.wipdays,a.ID   
 		 ,case when @prev_device = a.device then @cumtotal := @cumtotal + a.wipdays else @cumtotal := a.wipdays end as cumwipdays   
 		 ,case when @prev_device = a.device and a.keydev=1 then @qtytotal := @qtytotal + a.CUR_QTY else @qtytotal := a.CUR_QTY end as key_total   
 		 ,(@prev_device := a.device) as tempdev   
 		 , a.keydev  
 		, case when w1.device=a.device and w1.id=a.id then 1 else 0 end as one_dev  
        , pkg_size
        , flow
 	 from (  
 		 select @prev_device := null   
 		 ,@cumtotal := 0   
 		 ,@qtytotal := 0 
 	 ) i   
 	 join (   
 	 select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,   
 	 case when left(START_TIME,2)= ' '   then replace(START_TIME,' '  ,'' ) else START_TIME end  as START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,wipdays,ID,keydev,pkg_size,flow from   
 	(   
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
 	 ) a  
 	left join (select device,min(id) as id from wip w2 group by w2.device) w1 on w1.device=a.device and w1.id=a.id  
 )fr1 left join keydevices kd on fr1.device=kd.key_device  
 left join ( 
 			select device,total_wip, case when dd.loadboard is not null then 1 else 0 end as current_lb from ( 
 			select r.Tooling_Family,wip.Device,sum(wip.CUR_QTY) as dev_wip from wip wip  
 			left join (select device,Tooling_Family from route r   
 			group by Tooling_Family,device) r on r.device=wip.Device  
 			group by r.Tooling_Family,wip.Device 
 			) bb 
 			left join   
 			(select aa.tooling_family,sum(total_wip) as total_wip from  
 			(select r.Tooling_Family,wip.Device,sum(wip.CUR_QTY) as total_wip from wip wip  
 			left join (select device,Tooling_Family from route r  
 			group by Tooling_Family,device) r on r.device=wip.Device  
 			group by r.Tooling_Family,wip.Device  
 			) aa group by aa.tooling_family) cc on bb.tooling_family = cc.tooling_family  
 			left join (  
 				select * from (  
 				select case when cl.aux_type_TWSTATUS_i = '' then cl.hib_type_TWSTATUS_i else cl.aux_type_TWSTATUS_i end as loadboard  
 				from clattester_stationstatus cl  
				where substr(cl.tester_TWSTATUS_i,1,4)= 'TT55'  
 				and tempOnTstr_f<>'' ) aa  
 				group by  aa.loadboard 
 			) dd on dd.loadboard=cc.tooling_family 
 		) wip_total on wip_total.device=fr1.device  
 left join ( 
 			select device,capacity from (  
 			select r.Tooling_Family,wip.Device,round(sum(cap.capacity),2) as capacity_wip from wip wip  
 			left join (select device,Tooling_Family from route r  
 			group by Tooling_Family,device) r on r.device=wip.Device  
 			left join device_cap cap on cap.device=wip.device  
 			group by r.Tooling_Family,wip.Device  
 			) cc  
 			left join (  
 			select aa.Tooling_Family, sum(capacity) as capacity  from (  
 			select r.Tooling_Family,wip.Device,round(sum(cap.capacity),2) as capacity from wip wip  
 			left join (select device,Tooling_Family from route r  
 			group by Tooling_Family,device) r on r.device=wip.Device   
 			left join device_cap cap on cap.device=wip.device  
 			group by r.Tooling_Family,wip.Device) aa  
 			group by aa.tooling_family ) bb on bb.tooling_family = cc.tooling_family  
 		) cap on cap.device=fr1.device  
 left join (select wip.pkg_size,sum(wip.CUR_QTY) as total_wip_pkg from wip group by wip.pkg_size) gpkg on gpkg.pkg_size=fr1.pkg_size
 order by ID 
 )fn 
 where (cumwipdays<>9999 )  
 order by ID 
 )fr  order by ID 
 )
 ;
	 INTO OUTFILE 'C:/optimizer/data/wip.csv' 
	 FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
	 LINES TERMINATED BY '\n' 
); 