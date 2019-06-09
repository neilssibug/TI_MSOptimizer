#!/usr/bin/perl

=for
###################################################################################
TITLE: DB Connector Class
FILENAME:	dbConnector.pl
AUTHOR: Neil
DATE CREATED: 2016-01-21
TECHNOLOGY: PERL Script

MODIFIER: -:-
DATE MODIFIED: -:-
TECHNOLOGY: -:-

DETAILS:
Class for MSOpt MySQL and Oracle connection and returns it as an object
###################################################################################
=cut

package dbConnector;

use warnings;
use DBI;

#MYSQL
my $mysql_host = "localhost";
my $mysql_db = "optimizer";
my $mysql_user = "root";
my $mysql_pass = "root";
my $mysql_dns = "dbname=" . $mysql_db . ";host=" . $mysql_host;

#ORACLE
# - Create a dblink to connect all related Oracle database
$ENV{ORACLE_HOME} = 'C:/Oracle/Product/10.2';
$ENV{ORACLE_BIN} = 'C:/Oracle/Product/10.2/bin';
$ENV{ORACLE_LIB} = 'C:/Oracle/Product/10.2/lib';

my $oracle_host = "clgprpt.ticl.make.ti.com";
my $oracle_port = 1521;
my $oracle_sid = "clgprpt";
my $oracle_user = "rpt_user";
my $oracle_pass = "rptuser1";
my $oracle_dns = "host=$oracle_host;sid=$oracle_sid;port=$oracle_port";

#my $oracle = DBI -> connect("DBI:Oracle:host=$oracle_host;sid=$oracle_sid;port=$oracle_port", $oracle_user, $oracle_pass,{ RaiseError => 1 },) or die $DBI::errstr;
#my $oracle = DBI -> connect("DBI:Oracle:" . $oracle_dns, $oracle_user, $oracle_pass,{ RaiseError => 1 },) or die $DBI::errstr;

require Exporter;
@ISA = qw(Exporter);
@EXPORT = qw(MySQL Oracle);

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

sub MySQL
{
	my $mysql = DBI->connect( 'dbi:mysql:' . $mysql_dns, $mysql_user, $mysql_pass,{ RaiseError => 1 },) or die $DBI::errstr;
	
	return $mysql;
}

sub Oracle
{
	my $oracle = DBI -> connect("DBI:Oracle:" . $oracle_dns, $oracle_user, $oracle_pass,{ RaiseError => 1 },) or die $DBI::errstr;
	
	return $oracle;
}

1;

__END__