#!/usr/bin/perl

=for
###################################################################################
TITLE: generate Initial setup file
AUTHOR: Mike
DATE CREATED: 2015-10-22
TECHNOLOGY: VB Script

MODIFIER: Neil
DATE MODIFIED: 2016-01-21
TECHNOLOGY: PERL Script

DETAILS:
Generate new initial setup file
###################################################################################
=cut

package InitialSetup;

use warnings;

my $sqlplus = "c:/oracle/product/11.2/BIN/sqlplus.exe";
my $sqlloc = "c:/optimizer/sql";
my $smsdwsql = $sqlloc . "/smsdw_sql.sql";

require Exporter;
@ISA = qw(Exporter);
@EXPORT = qw(generateInitialSetup generateRTD_Curr_Future generateRTD_SMSDW generateSMSDW generatePKG_LoadBoard uploadInitialSetup);

$SIG{__DIE__} = sub { 
my $message = "DIE:" . shift;
print $message;
#LogMsg("=*=*=*=*\n".$message."\n=*=*=*=*".(localtime),"TPC_DIE_ERR_PGMUTILS_");
# CANNOT SEND EMAIL NOTIFICATION SINCE EMAIL_SUPPORT IS ON THE CONFIG FILE;
# CANNOT SEND TO ME(x0229958) SINCE DEV COULD BE REPLACED ANYTIME;
# SendEmail($tpc_support,"","***TPC_ERR_DIE_MSG***","",$message."<br/>FILE:PGMutils.pm<br/>AT SITE : $AT_site_id");  
};

####### CATCH WARNINGS AND LOG
$SIG{__WARN__} = sub { 
my $message = "WARN:" . shift;
print $message;
#LogMsg("=*=*=*=*\n".$message."\n=*=*=*=*".(localtime),"TPC_WARN_ERR_PGMUTILS_"); 
};

sub new 
{
	my $class = shift;
    my $self = {
        _local_folder 	=> shift,
		_dbh			=> shift,
    };
	
	bless $self, $class;
    return $self;
}

sub generateInitialSetup {
    my( $self ) = @_;    
	my $dbh = $self->{_dbh};   
	my $file = $self->{_local_folder} . "/data/initialsetup.csv";
	
	print "Generating Initial setup ...\n";
	
	# Deleting initialsetup.csv file
	if (-e $file) {
		unlink $file or die "Unable to unlink $file: $!";
	}
	
	# Creating prepare and execute SQL script to generate Initial Setup file
	$sql = " (select  'Machine_Family','Machine_Instance','Machine_Family_Name','Initial_Tool_Family','Initial_Certification')
			union 
			(  
			select coalesce(m.machine_family,'') as machine_family,aa.Machine_Instance,coalesce(m.Machine_Family_Name,'') as machine_family_name, 
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
			group by 1,2,3,4 
			INTO OUTFILE 'C:/optimizer/data/initialsetup.csv' 
			FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
			LINES TERMINATED BY '\n' 
			);";
			
	my $sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
	
	print "Generating Initial setup ... OK\n";
	
	#$self->{_local_folder} = $local_folder . "data/initialsetup.csv" if defined($local_folder);
    #return $self->{_local_folder};
}

sub generateRTD_Curr_Future {
	my( $self ) = @_;    
	my $dbh = $self->{_dbh};   
	my $file = $self->{_local_folder} . "/data/current_VS_future_VS_RTD.csv";
	
	print "Generating RTD current vs future ...\n";
	
	# Deleting current_VS_future_VS_RTD.csv file
	if (-e $file) {
		unlink $file or die "Unable to unlink $file: $!";
	}
	
	# Creating prepare and execute SQL script to generate generate rtd current vs future file
	$sql=" (select 'current_machine_instance','current_tooling_family','smsdw_machine_instance','smsdw_tooling_family','future_machine_instance','future_tooling_family' 
		 )union(
		 select * from (select cs.Machine_Instance as inital_machine_instance, 
		 cs.initial_tool_family , rtd.machine_instance as rtd_machine_instance, 
		 rtd.Tooling_Family as rtd_tooling_family,
		 app.machine_instance as future_machine_instance,app.Tooling_Family as future_tooling_family 
		 from  
		 (SELECT * FROM  ( select machine_family,Machine_Instance,machine_family as Machine_Family_Name,case when loadboard is null then '' else loadboard end as initial_tool_family, 
			 case when temp=25 then 1 
						 when temp>25 then 2 
			   when temp<25 then 3  
			 else 1     
			 end as initial_certification 
			from  (     
						  select loadboard,machine_family,machine_instance,temp 
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
														  where tempOnTstr_f <>''  and tester_TWSTATUS_i like 'TT55%' 
										  ) a   
			) rtd  
		  ) aa  
		   GROUP BY 1,2,3,4,5) cs 
		 left join 
		 (select * from (select t.tester_TWSTATUS_i as machine_instance,s.atss_loadboard as tooling_family,s.atss_testec_config as machine_family, 
		 case when s.atss_temp>85 then 3  when s.atss_temp >30 and s.atss_temp <=85 then 2 else 1 end as certification 
		 from (select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from ( 
		 select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i, max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i  from twstatus t where 
		 substr(t.tester_TWSTATUS_i,1,4)='TT55' 
		 group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc) aa 
		 group by 2) t left join smsdw s  on t.lot_TWSTATUS_i = s.lot) aa where machine_family like 'ET%' group by 1,2 
		 ) rtd 
		 on rtd.Machine_Instance = cs.Machine_Instance 
		   left join 
		   (select mac.device,mac.lot,mac.machine_instance,mac.lpt,coalesce(atss.tooling_family,'') as tooling_family ,atss.machine_family,coalesce(atss.atss_certification,'') as atss_certification  from ( 
		 select Next_deviceAlias_NEXTLOTINFO_i as device,LotId_NEXTLOTINFO_i as lot, 
		 case when    
						 substr(Tester_NEXTLOTINFO_i,1,4)= 'TT55' then concat('CLFTEA', substr(Tester_NEXTLOTINFO_i,5,2)) 
						 else Tester_NEXTLOTINFO_i    
		  end as machine_instance,Lpt_NEXTLOTINFO_i as lpt 
		 from test_nextlotinfo 
		 WHERE substr(Tester_NEXTLOTINFO_i,1,4)= 'TT55' 
		 ) mac 
		 left join 
		 (select spec_device_ATSSTESTSPEC_i as device, 
		 case when test_hw_board1_ATSSTESTSPEC_i <> 'null' then test_hw_board1_ATSSTESTSPEC_i 
		 when test_hw_board2_ATSSTESTSPEC_i <> 'null' then test_hw_board2_ATSSTESTSPEC_i 
		 when test_hw_board3_ATSSTESTSPEC_i <> 'null' then test_hw_board3_ATSSTESTSPEC_i 
		 when test_hw_board4_ATSSTESTSPEC_i <> 'null' then test_hw_board4_ATSSTESTSPEC_i 
		 when test_hw_board5_ATSSTESTSPEC_i <> 'null' then test_hw_board5_ATSSTESTSPEC_i 
		 when test_hw_board6_ATSSTESTSPEC_i <> 'null' then test_hw_board6_ATSSTESTSPEC_i 
		 when test_hw_cib_ATSSTESTSPEC_i <> 'null' then test_hw_cib_ATSSTESTSPEC_i 
		 when test_hw_cib1_ATSSTESTSPEC_i <> 'null' then test_hw_cib1_ATSSTESTSPEC_i 
		 when test_hw_cib2_ATSSTESTSPEC_i <> 'null' then test_hw_cib2_ATSSTESTSPEC_i 
		 when test_hw_cib3_ATSSTESTSPEC_i <> 'null' then test_hw_cib3_ATSSTESTSPEC_i 
		 when test_hw_hib_ATSSTESTSPEC_i <> 'null' then test_hw_hib_ATSSTESTSPEC_i 
		 when test_hw_hib1_ATSSTESTSPEC_i <> 'null' then test_hw_hib1_ATSSTESTSPEC_i 
		 when test_hw_hib2_ATSSTESTSPEC_i <> 'null' then test_hw_hib2_ATSSTESTSPEC_i 
		 when test_hw_hib3_ATSSTESTSPEC_i <> 'null' then test_hw_hib3_ATSSTESTSPEC_i 
		 end as tooling_family, 
		 tester_config_ATSSTESTSPEC_i as machine_family, 
		 case when substr(temp_type_ATSSTESTSPEC_i,6,2) =25 then 1    
						 when substr(temp_type_ATSSTESTSPEC_i,6,2) >25 then 2    
			   when substr(temp_type_ATSSTESTSPEC_i,6,2) <25 then 3 
			 else 1    
			 end as atss_certification 
		   FROM optimizer.atsstestspec 
		   where tester_config_ATSSTESTSPEC_i <> 'NULL' and tester_config_ATSSTESTSPEC_i like 'ET%') atss on mac.device=atss.device) app 
		   on app.machine_instance=cs.machine_instance) aaaa group by 1,2,3,4,5
			INTO OUTFILE 'C:/optimizer/data/current_VS_future_VS_RTD.csv'   
			FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY ''   
			LINES TERMINATED BY '\n' 
		 )";
	my $sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
		
	print "Generating RTD current vs future.. OK\n";
}

sub generateRTD_SMSDW {
	my( $self ) = @_;    
	my $dbh = $self->{_dbh};   
	my $file = $self->{_local_folder} . "/data/rtd_smsdw.csv";
	
	print "Generating RTD SMSDW ...\n";
	
	# Deleting rtd_smsdw.csv file
	if (-e $file) {
		unlink $file or die "Unable to unlink $file: $!";
	}
	
	# Creating prepare and execute SQL script to generate generate RTD SMSDW file
	$sql = "(select 'machine_instance','Tooling_Family')
		union 
		(select * from (select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as machine_instance,s.atss_loadboard as tooling_family 
		from smsdw s inner join twstatus t on t.lot_TWSTATUS_i = s.lot) aa group by 1,2 
		INTO OUTFILE 'C:/optimizer/data/rtd_smsdw.csv' 
		FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
		LINES TERMINATED BY '\n' 
		)";
		
	my $sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
	
	print "Generating RTD SMSDW... OK\n";
}

sub generateSMSDW {
	my( $self ) = @_;    
	my $dbh = $self->{_dbh};   
	my $file = $self->{_local_folder} . "/data/smsdw.csv";
	
	print "Generating SMSDW ...\n";
	
	# Deleting smsdw.csv file
	if (-e $file) {
		unlink $file or die "Unable to unlink $file: $!";
	}
	
	# Creating prepare and execute SQL script to generate SMSDW file
	$sql = "(select  'machine_instance','Tooling_Family','Machine_Family','Cerification')  
			union   
			(select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as machine_instance,s.atss_loadboard as tooling_family,s.atss_testec_config as machine_family,  
			case when s.atss_temp>85 then 3  when s.atss_temp >30 and s.atss_temp <=85 then 2 else 1 end as certification   
			from smsdw s inner join twstatus t on t.lot_TWSTATUS_i = s.lot  
			INTO OUTFILE 'C:/optimizer/data/smsdw.csv' 
			FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY '' 
			LINES TERMINATED BY '\n' 
			)";
		
	my $sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
	
	print "Generating SMSDW... OK\n";
}

sub generatePKG_LoadBoard {
	my( $self ) = @_;    
	my $dbh = $self->{_dbh};   
	my $file = $self->{_local_folder} . "/data/loadboard_by_package.csv";
	
	print "Generating package_by_loadboard...\n";
	
	# Truncate loadboard_by_package
	$sql = "truncate loadboard_by_package";
	my $sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
	
	# Upload to mysql database
	$sql = " insert into loadboard_by_package (tooling_family,package,total_wip) 
			 ( select * from ( 
				select w.tooling_family,w.package,sum(CUR_QTY) as total_wip from ( 
				(select r.tooling_family,wip.package,wip.cur_qty from 
				wip_cut wip 
				left join route r on wip.Device=r.device and wip.Package=r.package and wip.LPT=r.logpoint 
				where r.alternate=\"\" 
				group by wip.id) 
				union all 
				( 
				select aa.tooling_family,aa.package,aa.CUR_QTY from ( 
				select r.*,wip.CUR_QTY from wip_cut wip 
				left join route r on wip.Device=r.device and wip.Package=r.package and wip.LPT=r.logpoint 
				where r.alternate=\"\"  
				)aa 
				where aa.device in (select r.device from wip_cut wip 
				left join route r on wip.Device=r.device and wip.Package=r.package and wip.LPT=r.logpoint 
				where r.alternate<>\"\" ) group by aa.id)) w 
				group by w.tooling_family,w.package ) aa
				order by aa.total_wip desc
			 )";
	$sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
		
	# Deleting loadboard_by_package.csv file
	if (-e $file) {
		unlink $file or die "Unable to unlink $file: $!";
	}
	
	# Creating prepare and execute SQL script to generate loadboard_by_package.csv file
	$sql = " (select 'tooling_family', 'package', 'total_wip') 
			  union (	
			  select * from (  
				select w.tooling_family,w.package,sum(CUR_QTY) as total_wip from ( 
				(select r.tooling_family,wip.package,wip.cur_qty from  
				wip_cut wip 
				left join route r on wip.Device=r.device and wip.Package=r.package and wip.LPT=r.logpoint 
				where r.alternate=''  
				group by wip.id) 
				union all 
				( 
				select aa.tooling_family,aa.package,aa.CUR_QTY from ( 
				select r.*,wip.CUR_QTY from wip_cut wip 
				left join route r on wip.Device=r.device and wip.Package=r.package and wip.LPT=r.logpoint 
				where r.alternate=''  
				)aa 
				where aa.device in (select r.device from wip_cut wip 
				left join route r on wip.Device=r.device and wip.Package=r.package and wip.LPT=r.logpoint 
				where r.alternate<>'' ) group by aa.id)) w 
				group by w.tooling_family,w.package ) aa 
				order by aa.total_wip desc	 
			  INTO OUTFILE 'C:/optimizer/data/loadboard_by_package.csv' 
			  FIELDS ENCLOSED BY '' TERMINATED BY ',' ESCAPED BY ''  
			  LINES TERMINATED BY '\n' 
			  )";
	$sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
	
	print "Generating package_by_loadboard... OK\n";
}

sub uploadInitialSetup {
	print "Uploading initialsetup to mysql database...\n";
	my( $self ) = @_;    
	my $dbh = $self->{_dbh};   
	
	# Truncate initialsetup
	my $sql = "truncate initialsetup";
	my $sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
	
	# upload initialsetup to mysql database
	$sql = " insert into initialsetup  
			 ( 
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
			 )";
	$sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
	
	print "Uploading initialsetup to mysql database...OK\n";
}

1;