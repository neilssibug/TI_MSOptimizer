#!/usr/bin/perl

=for
###################################################################################
TITLE: update msopt tool website
FILENAME: update_msopt_tool.vbs
AUTHOR: Mike
DATE CREATED: 2015-09-08
TECHNOLOGY: VB Script

FILENAME: update_msopt_tool.pl
MODIFIER: Neil
DATE MODIFIED: 2016-01-21
TECHNOLOGY: PERL Script

DETAILS:
Update table rpttest_admin.optimizer_folders
###################################################################################
=cut

use warnings;
use DBI;

use Cwd  qw(abs_path);
use lib abs_path;
use dbConnector;

my $oracle = dbConnector->Oracle;


# (1) quit unless we have the correct number of command-line args
if ( @ARGV != 1 )
{
	print "\nUsage: update_msopt_tool.pl [folder]\n";
	exit;
}

# (1) we got one command line args, so assume they are the
# folder
$folder=$ARGV[0];
#print "This is passed argument $folder\n";

my $sql = "INSERT INTO rpttest_admin.optimizer_folders VALUES('" . $folder . "',SYSDATE,'ver.3.1')";
my $sth = $oracle->prepare($sql)
	or die "Couldn't prepare SQL statement: \n" . $oracle->errstr;
$sth->execute() 
	or die "Couldn't execute SQL statement: \n" . $sth->errstr;

1;

__END__