#!/usr/bin/perl

use warnings;

use Cwd  qw(abs_path);
use lib abs_path;
use dbConnector;
use InitialSetup;

my $mysql = dbConnector->MySQL;
my $oracle = dbConnector->Oracle;


#$initsetup = new InitialSetup("c:/optimizer",$mysql);
#$initsetup->generateInitialSetup();

__END__