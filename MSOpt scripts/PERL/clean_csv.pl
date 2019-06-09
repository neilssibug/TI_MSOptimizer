#!/usr/bin/perl

=Description
TITLE: clean_csv
AUTHOR: Neil
DETAILS:
VERSION: 1.3
	-Scripts to be run before executing AT.exe
	-Download keydevices from DB
	-Remove  multiple space at WIP.csv start_time field
	-Read CSV File

VERSION: 1.4
	-Read MAXWipDays
	-Dump input files to mysql database. mysql odbc connetion driver 5.1 needed
	-correct temperature in machines.csv
	-Create limited wip based on WIPdaysmax on MSOPTconfig file
	-Create initialsetup.csv
	-Limit wip for keydevices to target output only
	-remove start_time and machine instance

VERSION: 1.5
	-generate new machine_hours.csv
	-Ignore maxwipdays for regular device
	-add buffer to key devices target
	-Fix single digit tester
	-Download machines.csv  from DB

VERSION: 1.6
	-Generate new tooling family setup time

VERSION: 1.7
	-add initial setup.
=cut

use warnings;

use lib abs_path;
use dbConnector;
use InitialSetup;

my $mysql = dbConnector->MySQL;
undef $sth;


#BEGIN CONSTANT VARIABLES
use constant sql_ora => "select DEVICE_NAME,TARGET_AMOUNT from rpttest_admin.optimizer_key_devices where STATUS='CURRENT' order by LAST_UPDATE";
use constant key_buffer => 1.1; #add 10% to target output
use constant tooling_family_setup_time => 0.0007; #0.083

use constant ForReading => 1;
use constant ForWriting => 2;

undef $WIPdaysmax;
undef $DEFAULT_CAPACITY_PER_DAY_PER_TESTER;
undef $USE_DEFAULT_CAPACITY;
undef $queue_threshold;
undef $Queue_size_enable;
undef $delete_file;

my $skip_row = 0;
my $local_folder = "c:/optimizer";
my $local_input = "c:/optimizer/data";
my $sqlplus = "C:/oracle/product/11.2/BIN/sqlplus";
my $sqlloc = "c:/optimizer/sql";
my $machine_sql  = $sqlloc . "/machines.sql";
my $keydevices_sql = $sqlloc . "/keydevices.sql";
my $device_cap_sql = $sqlloc . "/device_cap.sql";
my $loadboardstatus_sql = $sqlloc . "/loadboard_status.sql";
my $package_size_sql = $sqlloc . "/package_size.sql";

my $shl_local_folder = "c:\\optimizer";
my $shl_local_input = "c:\\optimizer\\data";


#WScript.StdOut.Write("Creating keydevices.csv" & vbCrLf)
print "Creating keydevices.csv\n";
#Download_input_files 'download key_devices, and external oracle databases
download_input_files(); #download key_devices, and external oracle databases


#upload csv files to local mysql database
print "Uploading csv files\n";
upload_mysql(); #upload input files
print "Uploading csv files Complete\n";
print "Processing config file\n";
get_config(); #capture config files
print "Processing config file.. ok\n";

if (-e "$local_input/wip.csv")
{
	$shell_cmd = "cmd.exe /c copy  $shl_local_input\\wip.csv  $shl_local_input\\wip_full.csv /Y ";
	system($shell_cmd);
}

$at_sql = " ";
if ($Queue_size_enable == 1) {
	if ($USE_DEFAULT_CAPACITY == 1) {
		$at_sql= " and below_pkg_queue=0 and below_lb_queue=0 ";
	} else {
		$at_sql = " and queue > " . $queue_threshold;
	}
}

print "Generating wip...\n";
print "sql: " . $at_sql . "\n";

$delete_file = "$local_input/wip.csv";
if (-e $delete_file)
{
	unlink $delete_file or die "Unable to unlink $delete_file: $!";
}

#process cumulative wip days and output new wip.csv file
$sql = "(select 'LOT','DEVICE','PACKAGE','PIN','CUR_QTY','WEIGHT','LOT_NAME','LPT','OPN','TOTAL_PLAN_CT','CUM_PLAN_CT','LOT_AGE','START_TIME','SNAPSHOOT_TIME','MACHINE_INSTANCE') 
 union 
 ( select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,CASE WHEN LPT<>'4420' THEN '4410' ELSE '4420' END AS LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE from ( 
 select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,wipdays,ID,cumwipdays,key_total,tempdev,keydev,target,queue,current_lb from ( 
 select LOT,fr1.DEVICE,fr1.PACKAGE,fr1.PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT, 
 CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,wipdays,fr1.ID,current_lb 
 ,case when keydev=1 or current_lb=1 then 9999 else if(fr1.device in (select r.device from route r where r.tooling_family in (select atss_loadboard from smsdw)),9999,round((wip_total.total_wip/round((if(cap.capacity is null or cap.capacity = '',(select avg(capacity) from device_cap),cap.capacity )),2)),2))  end as queue   
 ,case when keydev=1 and key_total>(kd.Target * 1.1 ) and one_dev=0 then 9999 when keydev=1 then 0.1 else round( if(fr1.device in (select r.device from route r where r.tooling_family in (select atss_loadboard from smsdw)),0.1,cumwipdays),2) end as cumwipdays,key_total,tempdev,keydev,target * 1.1 as target   
 ,case when keydev=1 then 0 when wip_total.total_wip<(select value from msoptconfig where trim(configname) = 'DEFAULT_CAPACITY_PER_DAY_PER_TESTER' ) then 1 else 0 end as below_lb_queue
 ,case when keydev=1 then 0 when gpkg.total_wip_pkg<(select value from msoptconfig where trim(configname) = 'DEFAULT_CAPACITY_PER_DAY_PER_TESTER_PKG_SZE') then 1 else 0 end as below_pkg_queue
 from (  
 	select a.LOT,a.DEVICE,a.PACKAGE,a.PIN,a.CUR_QTY,a.WEIGHT,a.LOT_NAME,a.LPT,a.OPN,a.TOTAL_PLAN_CT,a.CUM_PLAN_CT,a.LOT_AGE, case when start_time<>'' then date_format(CONVERT_TZ(str_to_date(trim(start_time),'%m/%d/%Y %H:%i:%s'),'-06:00','+08:00'),'%m/%d/%Y %H:%i:%s') else '' end as start_time,a.SNAPSHOOT_TIME, MACHINE_INSTANCE,a.wipdays,a.ID   
 		 ,case when \@prev_device = a.device then \@cumtotal := \@cumtotal + a.wipdays else \@cumtotal := a.wipdays end as cumwipdays   
 		 ,case when \@prev_device = a.device and a.keydev=1 then \@qtytotal := \@qtytotal + a.CUR_QTY else \@qtytotal := a.CUR_QTY end as key_total   
 		 ,(\@prev_device := a.device) as tempdev   
 		 , a.keydev  
 		, case when w1.device=a.device and w1.id=a.id then 1 else 0 end as one_dev  
 	 from (  
 		 select \@prev_device := null   
 		 ,\@cumtotal := 0   
 		 ,\@qtytotal := 0 
 	 ) i   
 	 join (   
 	 select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,   
 	 case when left(START_TIME,2)= \" \"  then replace(START_TIME,\" \" ,\"\" ) else START_TIME end  as START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,wipdays,ID,keydev from   
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
 left join (select wip.package,sum(wip.CUR_QTY) as total_wip_pkg from wip group by wip.package) gpkg on gpkg.package=fr1.package
 order by ID 
 )fn 
 where (cumwipdays!=9999 ) " . $at_sql . " order by ID 
 )fr  order by ID 
	 INTO OUTFILE 'C:/optimizer/data/wip.csv' 
	 FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
	 LINES TERMINATED BY '\n' 
)";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;

#wip cutted upload
$sql = "truncate wip_cut";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;

$sql  =	"load data local infile 'C:/optimizer/data/wip.csv' into table wip_cut fields terminated by ',' lines terminated by '\n'
		 IGNORE 1 LINES (LOT,Device,Package,Pin,CUR_QTY,Weight,LOT_name,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE)";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;
print "Generating wip..  OK\n";

print "Generating machine.csv ... \n";

#backup machines.csv
if (-e "$local_input/machines.csv")
{
	$shell_cmd = "cmd.exe /c copy $shl_local_input\\machines.csv $shl_local_input\\machines_full.csv /Y ";
	system($shell_cmd);
}

#process to correct the certification on machines.csv
$delete_file = "$local_input/machines.csv";
if (-e $delete_file)
{
	unlink $delete_file or die "Unable to unlink $delete_file: $!";
}

$sql = "(select 'Machine_Family','Machine_Instance','Machine_Family_Name','Certification') 
 union 
 ( select * from (
 select m.machine_family,m.machine_instance,m.machine_family as machine_family_name, 
 if( r.Certification is null or  r.Certification='', 
 	if(ini.Initial_Certification is null or ini.Initial_Certification='', 
 		if(rtd.Certification is null or rtd.Certification ='',1	 
         ,rtd.Certification) 
 	,ini.Initial_Certification) ,r.Certification  
 ) as certification 
  from 
 machines m 
 left join 
 (select coalesce(m.machine_family,'') as machine_family,aa.Machine_Instance,coalesce(m.Machine_Family_Name,'') as machine_family_name,
 aa.initial_tooling_family, 
 case when aa.initial_tooling_family='' then '' else coalesce(if(m.Machine_Family_Name is null,'',if(r.Certification is null, aa.initial_certification,r.Certification)),'') end as Initial_Certification from (  
  select coalesce(trim(s.atss_testec_config),'') as machine_family,  
  coalesce(trim(case when substr(t.tester_TWSTATUS_i,7,1)=0 then concat('CLFTEA',substr(t.tester_TWSTATUS_i,8,1)) else  
  t.tester_TWSTATUS_i end ),'') as machine_instance,coalesce(trim(s.atss_testec_config),'') as machine_family_name,coalesce(trim(s.atss_loadboard),'') as initial_tooling_family,  
   case when s.atss_temp>85 then 3  when s.atss_temp >30 and s.atss_temp <=85 then 2 else 1  end as initial_certification from (  
   select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from (   
   select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i, max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i  from twstatus t where   
   substr(t.tester_TWSTATUS_i,1,4)='TT55'   
   group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc) aa  
   group by 2) t  
   left join smsdw s on trim(s.lot)= trim(t.lot_TWSTATUS_i) 
   group by 2  
   ) aa left join machines m on m.Machine_Instance= aa.machine_instance 
   left join route r on r.machine_family=m.machine_family and r.tooling_family=aa.initial_tooling_family 
   group by 1,2,3,4)ini on ini.machine_instance = m.machine_instance 
 left join ( select loadboard,machine_family,case when substr(machine_instance,7,1)=0 then concat(substr(machine_instance,1,6),substr(machine_instance,8,1)) else machine_instance end as machine_instance , 
 	case when temp>=30 and temp<=85 then 3 
 		when temp>85 then 2 
         else 1 end as certification 
 	 from (  
 	 select aux_type_TWSTATUS_i,hib_type_TWSTATUS_i, 
 	 case when aux_type_TWSTATUS_i='' then hib_type_TWSTATUS_i   
 		else aux_type_TWSTATUS_i  
 	 	end as loadboard  
 	 ,testerConfig_CMMSTESTERS_i as machine_family, lotOnTstr_f, substr(tempOnTstr_f,6,2) as temp  
 	 ,case when  
 		substr(tester_TWSTATUS_i,1,4)= 'TT55' then concat('CLFTEA', substr(tester_TWSTATUS_i,5,2))  
 	 	else tester_TWSTATUS_i  
 	 end as machine_instance  
 	 ,tstArea_TESTERS_i  
 	 from clattester_stationstatus  
 	 where tempOnTstr_f <>''  
 	 ) a)rtd on rtd.machine_instance=m.machine_instance 
 left join (select machine_family,tooling_family,Certification from route group by 1,2,3) r on r.Machine_Family=m.Machine_Family and r.tooling_family=ini.initial_tooling_family 
 where m.machine_family in (select machine_family from route) )gp group by 1,2,3  
 INTO OUTFILE 'C:/optimizer/data/machines.csv' 
 FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
 LINES TERMINATED BY '\n' 
)";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;

#machines 
$sql = "truncate machines";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;

$sql  =	"load data local infile \"C:/optimizer/data/machines.csv\" into table machines fields terminated by ',' lines terminated by '\n'
		 IGNORE 1 LINES (Machine_Family,Machine_Instance,Machine_Family_Name,Certification)";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;
print "Generating machine.csv ...  OK\n";

print "Generating machine_hours.csv ...\n ";
#populate machine hours
$delete_file = "$local_input/machine_hours.csv";
if (-e $delete_file)
{
	unlink $delete_file or die "Unable to unlink $delete_file: $!";
}

$sql = "(select 'machine_instance','hours') 
	union 
	(select machine_instance, case when Machine_Instance in ('CLFTEA90','CLFTEA91','CLFTEA95','CLFTEA98','CLFTEA99') then 0 else 48 end as hours from machine_hours 
	 INTO OUTFILE 'C:/optimizer/data/machine_hours.csv' 
	 FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
	 LINES TERMINATED BY '\n' 
	)";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;
print "Generating machine_hours.csv ..  OK\n";

print "Generating tooling.csv ..  \n";
#backup wip to wip_full and delete old wip

if (-e "$local_input/tooling.csv")
{
	$shell_cmd = "cmd.exe /c copy $shl_local_input\\tooling.csv $shl_local_input\\tooling_full.csv /Y ";
	system($shell_cmd);
}

$delete_file = "$local_input/tooling.csv";
if (-e $delete_file)
{
	unlink $delete_file or die "Unable to unlink $delete_file: $!";
}

#tooling
$sql = " (select 'tooling_family','Tooling_Instance', 'tooling_family_name','certification') 
 union ( 
 select tool.tooling_family,tool.tooling_instance,tool.Tooling_Family_Name,if (m.Certification is null,if(cs.Initial_Certification is null,tool.Certification,cs.Initial_Certification),m.Certification) as initial_certification  
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
   group by 1,2,3,4 
  INTO OUTFILE 'C:/optimizer/data/tooling.csv'  
  FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
  LINES TERMINATED BY '\n' 
  )";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;

#tooling
$sql = "truncate tooling";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;
	
$sql  =	"load data local infile \"C:/optimizer/data/tooling.csv\" into table tooling fields terminated by ',' lines terminated by '\n'
		 IGNORE 1 LINES (tooling_Family,Tooling_Instance,Tooling_Family_Name,Certification)";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;

print "Generating tooling.csv ..  OK\n";

print "Generating wip---Running external module InitialSetup.pm...  \n";
#run external script to run insert machine instance from initialsetup (current setup on the floor) to wip
#create a class for generate_initial_setup.vbs, instantiate on the constructor

# Create object for InitialSetup
$initialsetup = new InitialSetup($local_folder, $mysql);

# generateInitialSetup 
$initialsetup->generateInitialSetup();

# generateRTD_Curr_Future 
$initialsetup->generateRTD_Curr_Future();

# generateRTD_SMSDW 
$initialsetup->generateRTD_SMSDW();

#generateSMSDW 
$initialsetup->generateSMSDW();

#generatePKG_LoadBoard 
$initialsetup->generatePKG_LoadBoard();

#uploadInitialSetup
$initialsetup->uploadInitialSetup();

print "Generating wip---Running external module InitialSetup.pm...  OK\n";

print "Generating toolingfamily_setuptime.csv ...  \n";
#generate tooling_family_setup_time

$delete_file = "$local_input/toolingfamily_setuptime.csv";
if (-e $delete_file)
{
	unlink $delete_file or die "Unable to unlink $delete_file: $!";
}

#print $tooling_family_setup_time;
$sql = "(select 'Tooling_Family','Setup_time')  
	 UNION 
	(select tooling_family, " . tooling_family_setup_time . " as setup_time from toolingfamily_setuptime 
		 INTO OUTFILE 'C:/optimizer/data/toolingfamily_setuptime.csv' 
	 FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
	 LINES TERMINATED BY '\n' 
	)";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;
	
print "Generating toolingfamily_setuptime.csv ...  OK\n";

print "Generating Uploading keydevices.csv ... \n";
#keydevices upload to mysql
$sql = "truncate keydevices";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;
	
$sql  =	"load data local infile \"C:/optimizer/data/keydevices.csv\" into table keydevices fields terminated by ',' lines terminated by '\n'
		IGNORE 1 LINES (Key_Device,TARGET)";
$sth = $mysql->prepare($sql)
	or die "Couldn't prepare SQL statement: " . $mysql->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: " . $sth->errstr;
print "Generating Uploading keydevices.csv ...  OK\n";


sub download_input_files
{

=for
	 ' download directly from database keydevices
	on error resume next
	Dim strCon
	strCon ="Provider=OraOLEDB.Oracle;Data Source=CLARK_MSOPT;User ID=""rpt_user"";Password=""rptuser1"""

	Dim oCon: Set oCon = CreateObject("ADODB.Connection")
	Dim oRs: Set oRs = CreateObject("ADODB.Recordset")
	Dim oRm: Set oRs = CreateObject("ADODB.Recordset")
	oCon.Open strCon
	oCon.Open strCon
	oRs.CommandType = adCmdText
	oRs.CursorLocation=adUseClient
	oRs.CursorType=adOpenDynamic
	oRs.LockType=adLockBatchOptimistic

	Set oRs = oCon.Execute(sql_ora)
	
	'if not oRs.eof and not oRs.bof  then
REM		if objFSO.FileExists(local_input & "\keydevices.csv")   then
REM			Set objFilek = objFSO.OpenTextFile(local_input & "\keydevices.csv", 2, True)
REM		else
REM			Set objFilek = objFSO.CreateTextFile(local_input & "\keydevices.csv",True)
REM		end if
REM		
REM		objFilek.Close
REM		strNewContent1 = strNewContent1 & "Key_Device,TARGET" & vbCrLf
REM		oRs.movefirst
REM		WScript.StdOut.Write("Entering LOOP" & vbCrLf)
REM		Do Until  oRs.EOF
REM			strNewContent1 = strNewContent1 & oRs.Fields(0).Value & "," & oRs.Fields(1).Value & vbCrLf
REM			oRs.MoveNext
REM			WScript.StdOut.Write(oRs.Fields(0).Value & vbCrLf)
REM			echo strNewContent1
REM		loop
	
REM		Set objFilek = objFSO.OpenTextFile(local_input & "\keydevices.csv",ForWriting) ' remove all contents
REM		objFilek.Write strNewContent1 ' write key devices to keydevices.csv data
REM		objFilek.Close	
	'end if
	
	
	oCon.Close
	Set oRm = Nothing
	Set oRs = Nothing
	Set oCon = Nothing
=cut

	# using sqlplus
	# create machine.csv file from original query 
	print "$machine_sql...\n";
	$shell_cmd = "cmd.exe /c $sqlplus -s rpt_user/rptuser1\@CLARK_MSOPT \@$machine_sql" ;
	system($shell_cmd);
		
	# generate keydevices
	print "$keydevices_sql...\n";
	$shell_cmd = "cmd.exe /c $sqlplus -s  rpt_user/rptuser1\@CLARK_MSOPT \@$keydevices_sql";
	system($shell_cmd);
	
	#get package size from oracle to csv
	print "$package_size_sql...\n";
	$shell_cmd = "cmd.exe /c $sqlplus -s  rpt_user/rptuser1\@CLARK_MSOPT \@$package_size_sql";
	system($shell_cmd);
	
	#create loadboard_status.sql
	print "$loadboardstatus_sql...\n";
	$shell_cmd = "cmd.exe /c $sqlplus -s rpt_user/rptuser1\@CLARK_MSOPT \@$loadboardstatus_sql";
	system($shell_cmd);
	
	#generate device capacity
	print "$device_cap_sql...\n";
	$shell_cmd = "cmd.exe /c $sqlplus -s  rpt_user/rptuser1\@CLARK_MSOPT \@$device_cap_sql";
	system($shell_cmd);
}

sub upload_mysql
{
	#dump input files to database
	#route
	$sql  = "truncate route"; 
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile \"C:/optimizer/data/route.csv\" into table route fields terminated by ',' lines terminated by '\n' 
			IGNORE 1 LINES (RouteName,Logpoint,Operation,LPT_Desc,Opn_Desc,Device,Package,Pin,alternate,Setup,PPH,Machine_Family,Tooling_Family,ToolQTY,Certification)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#machines 
	$sql = "truncate machines";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile \"C:/optimizer/data/machines.csv\" into table machines fields terminated by ',' lines terminated by '\n'
			 IGNORE 1 LINES (Machine_Family,Machine_Instance,Machine_Family_Name,Certification)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#wip_full
	$sql = "truncate wip";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	
	$sql  =	"load data local infile \"C:/optimizer/data/wip.csv\" into table wip fields terminated by ',' lines terminated by '\n'
			IGNORE 1 LINES (LOT,Device,Package,Pin,CUR_QTY,Weight,LOT_name,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#toolingfamily_setuptime
	$sql = "truncate toolingfamily_setuptime";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	
	$sql  =	"load data local infile \"C:/optimizer/data/toolingfamily_setuptime.csv\" into table toolingfamily_setuptime fields terminated by ',' lines terminated by '\n'
			IGNORE 1 LINES (Tooling_Family,Setup_time)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#tooling
	$sql = "truncate tooling";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	
	$sql  =	"load data local infile \"C:/optimizer/data/tooling.csv\" into table tooling fields terminated by ',' lines terminated by '\n'
			IGNORE 1 LINES (tooling_Family,Tooling_Instance,Tooling_Family_Name,Certification)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#machine_hours
	$sql = "truncate machine_hours";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile \"C:/optimizer/data/machine_hours.csv\" into table machine_hours fields terminated by ',' lines terminated by '\n' 
			IGNORE 1 LINES (Machine_instance,Hours)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
			
	#key_pin_package
	$sql = "truncate key_pin_package";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile \"C:/optimizer/data/key_pin_package.csv\" into table key_pin_package fields terminated by ',' lines terminated by '\n'
			IGNORE 1 LINES (Key_Package,PIN,Target)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
			
	#key_pin_package
	$sql = "truncate key_package";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile \"C:/optimizer/data/key_package.csv\" into table key_package fields terminated by ',' lines terminated by '\n'
			IGNORE 1 LINES (Key_Package,Target)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#msoptconfig
	$sql = "truncate msoptconfig";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile \"C:/optimizer/data/MSOPTconfig.csv\" into table msoptconfig fields terminated by ',' lines terminated by '\r\n'
			IGNORE 1 LINES (configname,value)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#clatTester_StationStatus, Current setup
	$sql = "truncate clattester_stationstatus";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile \"C:/optimizer/clattester_stationstatus\" into table clattester_stationstatus fields terminated by '|' lines terminated by '\n'
			(aux_TWSTATUS_i,aux_type_TWSTATUS_i,cib_TWSTATUS_i,cib_type_TWSTATUS_i,devAliasOnTstr_f,devid_LOT_r,downCodeDescription_CMMSTESTERS_i,downCode_CMMSTESTERS_i,handler_TWSTATUS_i,handler_group_TWSTATUS_i,handler_type_TWSTATUS_i,hib_TWSTATUS_i,hib_type_TWSTATUS_i,lotOnTstr_f,pinOnTester_f,program_TWSTATUS_i,tempOnTstr_f,test_hw_handler_ATSSTESTSPEC_i,test_hw_hib_ATSSTESTSPEC_i,testerConfig_CMMSTESTERS_i,tester_TWSTATUS_i,tester_config_ATSSTESTSPEC_i,tester_type_TWSTATUS_i,up_UPSTATES_i,last_activity_f,tstArea_TESTERS_i,test_hw_board1_ATSSTESTSPEC_i,test_cell_DEVAREA_i,handler_TubeTray,devOnTstr_f,logptid_TWSTATUS_i,temp_type_lot_ctl_val_ATSSTESTSPEC_i,HandlerConfig_HANDLER_INFO_i,Setup_Change_f,NextLot_Trans_dttm_t,timestamp_CMMSTESTERS_i,test_niche_ATSSTESTSPEC_i,pkg_ATSSTESTSPEC_i,pkgOnTstr_f,nicheOnTstr_f)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#TEST_NEXTLOTINFO, Current setup application updates
	$sql = "truncate TEST_NEXTLOTINFO";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile \"C:/optimizer/TEST_NEXTLOTINFO\" into table TEST_NEXTLOTINFO fields terminated by '|' lines terminated by '\n'
			(User_NEXTLOTINFO_i,Next_deviceAlias_NEXTLOTINFO_i,LotId_NEXTLOTINFO_i,Lpt_NEXTLOTINFO_i,Tester_NEXTLOTINFO_i,Latest_NEXTLOTINFO_i,Trans_dttm_NEXTLOTINFO_i,dummy1,dummy2,dummy3,dummy4)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#atss specs 
	$sql = "truncate ATSSTESTSPEC";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql  =	"load data local infile 'C:/optimizer/ATSSTESTSPEC' into table ATSSTESTSPEC fields terminated by '|' lines terminated by '\n' 
			(spec_device_ATSSTESTSPEC_i,sap_material_ATSSTESTSPEC_i,sbe_ATSSTESTSPEC_i,sbe_1_ATSSTESTSPEC_i,pkg_ATSSTESTSPEC_i,pin_ATSSTESTSPEC_i,pkg_group_ATSSTESTSPEC_i,demand_ATSSTESTSPEC_i, 
			sub_flow_name_ATSSTESTSPEC_i, opn_name_ATSSTESTSPEC_i,osn_ATSSTESTSPEC_i,setno_ATSSTESTSPEC_i,sequence_ATSSTESTSPEC_i,tester_config_ATSSTESTSPEC_i,test_hw_ATSSTESTSPEC_i, 
			test_hw_board1_ATSSTESTSPEC_i,test_hw_board2_ATSSTESTSPEC_i,test_hw_board3_ATSSTESTSPEC_i,test_hw_board4_ATSSTESTSPEC_i,test_hw_board5_ATSSTESTSPEC_i,test_hw_board6_ATSSTESTSPEC_i, 
			test_hw_cib_ATSSTESTSPEC_i,test_hw_cib1_ATSSTESTSPEC_i,test_hw_cib2_ATSSTESTSPEC_i,test_hw_cib3_ATSSTESTSPEC_i,test_hw_ent_grp_ATSSTESTSPEC_i,test_hw_handler_ATSSTESTSPEC_i, 
			test_hw_handler1_ATSSTESTSPEC_i,test_hw_handler2_ATSSTESTSPEC_i,test_hw_hib_ATSSTESTSPEC_i,test_hw_hib1_ATSSTESTSPEC_i,test_hw_hib2_ATSSTESTSPEC_i,test_hw_hib3_ATSSTESTSPEC_i, 
			test_prog_ATSSTESTSPEC_i,test_prog_tester_ATSSTESTSPEC_i,test_niche_ATSSTESTSPEC_i, 
			temp_type_ATSSTESTSPEC_i,temp_type_lot_ctl_val_ATSSTESTSPEC_i , test_temperature_ATSSTESTSPEC_i,handler_config_ATSSTESTSPEC_i, 
			handler_config1_ATSSTESTSPEC_i,handler_config2_ATSSTESTSPEC_i , bi_time_to_test_ATSSTESTSPEC_i, test_prog_lot_ctl_val_ATSSTESTSPEC_i ,test_type_ATSSTESTSPEC_i, 
			handler_config1_ATSSTESTSPEC_i,handler_config2_ATSSTESTSPEC_i,bi_time_to_test_ATSSTESTSPEC_i, 
			test_prog_lot_ctl_val_ATSSTESTSPEC_i,test_type_ATSSTESTSPEC_i,dummy0_ATSSTESTSPEC_i,dummy1_ATSSTESTSPEC_i,dummy2_ATSSTESTSPEC_i,dummy3_ATSSTESTSPEC_i,dummy4_ATSSTESTSPEC_i, 
			dummy5_ATSSTESTSPEC_i,dummy6_ATSSTESTSPEC_i,dummy7_ATSSTESTSPEC_i,dummy8_ATSSTESTSPEC_i,dummy9_ATSSTESTSPEC_i,isgood_sps06_ATSSTESTSPEC_i,isdeleted_mm06_ATSSTESTSPEC_i,industry_sector_ATSSTESTSPEC_i,max_compattr_date_ATSSTESTSPEC_i)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#testware status
	#sql="truncate twstatus"
	#keep all data
	#myrs.open sql,mycn
	$sql =" load data local infile 'C:/optimizer/data/twstatus' into table twstatus fields terminated by '|' lines terminated by '\n' 
			(tester_TWSTATUS_i,tester_type_TWSTATUS_i,handler_TWSTATUS_i,handler_type_TWSTATUS_i,handler_group_TWSTATUS_i,hib_TWSTATUS_i, 
			hib_type_TWSTATUS_i,cib_TWSTATUS_i,cib_type_TWSTATUS_i,aux_TWSTATUS_i,aux_type_TWSTATUS_i,lot_TWSTATUS_i, program_TWSTATUS_i, 
			probecard_TWSTATUS_i,last_update_TWSTATUS_i,last_activity_TWSTATUS_i,secs_idle_TWSTATUS_i) ";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

	#package size
	$sql = "truncate package_size";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;
	$sql = "load data local infile \"C:/optimizer/extn_files/pkgsize.csv\" into table package_size fields terminated by ',' lines terminated by '\r\n'
			IGNORE 1 LINES (PACKAGE,PIN,PKG_GRP,PKG_SIZE,PKGTYPE,PIN_PKG)";
	$sth = $mysql->prepare($sql)
		or die "Couldn't prepare SQL statement: " . $mysql->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: " . $sth->errstr;

}

sub get_config
{
	my $file = "c:/optimizer/data/MSOPTconfig.csv";
	 
	open(my $data, '<', $file) or die "Could not open '$file' $!\n";
	close($file);
	 
	while (my $line = <$data>) {
		chomp $line;

		#my @fields = split "," , $line;
		#print "$fields[0] = $fields[1] \n";
		
		my ($key, $value) = split "," , $line;
		#print "$key = $value\n";
		
		if($key eq "Queue_size_enable") 
		{
			$Queue_size_enable = $value;
			print "$key = $value \n";
		}
		
		if($key eq "queue_threshold") 
		{
			$queue_threshold = $value;
			print "$key = $value \n";
		}
		
		if($key eq "DEFAULT_CAPACITY_PER_DAY_PER_TESTER")
		{
			$DEFAULT_CAPACITY_PER_DAY_PER_TESTER = $value;
			print "$key = $value \n";
		}
				
		if($key eq "USE_DEFAULT_CAPACITY") 
		{
			$USE_DEFAULT_CAPACITY = $value;
			print "$key = $value \n";
		}
		
		if($key eq "WIPdaysmax") 
		{
			$WIPdaysmax = $value;
			print "$key = $value \n";
		}		
	}
}

__END__