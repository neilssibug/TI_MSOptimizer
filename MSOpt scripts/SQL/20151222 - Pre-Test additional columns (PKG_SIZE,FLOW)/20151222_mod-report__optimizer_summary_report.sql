 (select 'pkgtype', 'tester_req','EDGE_LB','pkg_size','Device_Name','Package','expected_output_2_days','expected_avg_output_per_day'  
 ,'cap_per_day','wip_post_WB','wip_ft1','wip_ft2','POST_WB_QUEUE','TEST_QUEUE_FT1','TEST_QUEUE_FT2','Total_queue_1_tester')  
  union   
 (SELECT pkg.flow, sol.Machine_Family_Name as tester_req,  
 sol.Tooling_Family_Name as EDGE_LB  
 ,pkg.pkg_size, sol.Device_Name , pkg.package as package  
 , sum(sol.Quantity) as expected_output_2_days  
 , sum(sol.Quantity)/2 as expected_avg_output_per_day  
 , round(avg(cap.capacity),2) as cap_per_day   
 , COALESCE((wip_postwb.total_wip),0) as wip_post_WB  
 , COALESCE((wip_ft1.total_wip),0) as wip_ft1     
 , COALESCE((wip_ft2.total_wip),0) as wip_ft2  
 , COALESCE(round((wip_postwb.total_wip)/avg(cap.capacity),2),0) as POST_WB_QUEUE  
 , COALESCE(round((wip_ft1.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT1  
  , COALESCE(round((wip_ft2.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT2  
 , coalesce((COALESCE((wip_postwb.total_wip),0) + COALESCE((wip_ft1.total_wip),0) + COALESCE((wip_ft2.total_wip),0)) / round(avg(cap.capacity),2),0)  as total_wip  
 FROM solution sol  
 left join device_cap cap on cap.device=sol.Device_Name  
   left join wip pkg on pkg.device=sol.device_name and pkg.lot_name = sol.Lot_Name     
 left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2))  
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name  
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name  
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name  
 group by pkg.package, sol.Tooling_Family_Name,sol.Machine_Family_Name,sol.Device_Name  
 )
 ;
   INTO OUTFILE 'C:/optimizer/output/optimizer_summary_report.csv'   
  	FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY ''  
  	LINES TERMINATED BY '\n'   
)