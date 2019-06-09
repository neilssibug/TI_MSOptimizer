#!/usr/bin/perl

=for
###################################################################################
TITLE: update msopt tool website
FILENAME: update_msopt_tool.vbs
AUTHOR: Mike
DATE CREATED: 2015-09-08
TECHNOLOGY: VB Script

MODIFIER: Neil
DATE MODIFIED: 2016-01-21
TECHNOLOGY: PERL Script

DETAILS:
Update table rpttest_admin.optimizer_folders
###################################################################################
=cut

package updMSOptTool;

use warnings;

require Exporter;
@ISA = qw(Exporter);
@EXPORT = qw(update);

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
		_folder			=> shift,
		_dbh			=> shift,
    };
	
	bless $self, $class;
    return $self;
}

sub update
{
	my( $self ) = @_;    
	my $folder = $self->{_folder};
	my $dbh = $self->{_dbh};  
	
	my $sql = "INSERT INTO rpttest_admin.optimizer_folders VALUES('" . $folder . "',SYSDATE,'ver.3.1')";
	my $sth = $dbh->prepare($sql)
		or die "Couldn't prepare SQL statement: \n" . $dbh->errstr;
	$sth->execute() 
		or die "Couldn't execute SQL statement: \n" . $sth->errstr;
}

1;

__END__