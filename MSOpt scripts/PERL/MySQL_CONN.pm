#!/usr/bin/perl

=for
###################################################################################
TITLE: MySQL Connection Class
AUTHOR: Neil
DATE CREATED: 2016-01-21
TECHNOLOGY: PERL Script

MODIFIER: -:-
DATE MODIFIED: -:-
TECHNOLOGY: -:-

DETAILS:
Class for MySQL Connection and returns a Connection object
###################################################################################
=cut

package MySQL_CONN;

use warnings;
use DBI;

#MYSQL connection 

my $mysql_host = "localhost";
my $mysql_db = "optimizer";
my $mysql_user = "root";
my $mysql_pass = "root";
my $mysql_dns = "dbi:mysql:dbname=" . $mysql_db . ";host=" . $mysql_host;

sub new
{
	my $dbh = DBI->connect(          
    $mysql_dns, 
    $mysql_user,                          
    $mysql_pass,                          
    { RaiseError => 1 },         
	) or die $DBI::errstr;
	
	#bless $self, $dbh;
	#return $self;
	return $dbh;
}

1;