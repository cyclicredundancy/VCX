#!/usr/bin/perl -w
use strict;
use warnings;

#use IO::Socket;
#my $sock = new IO::Socket::INET (
#      PeerAddr => '127.0.0.1',
#      PeerPort => 9876,
#      Proto => 'tcp',
#      );
#die "Socket could not be created. Reason: $!\n" unless $sock;
#foreach (1 .. 10) {
#   print $sock "Msg $_: How are you?\n";
#    
#}
#close ($sock); 
#

use IO::Socket::INET;
use IO::Select;

my $srv = shift or die "Error: server name is missing\n";

my $pt = shift or die "Error: port is missing\n";

my $skt = IO::Socket::INET->new(
        'PeerAddr' => $srv, 
        'PeerPort' => $pt,
        'Proto' => 'tcp'
        ) or die "Error: Unable to create socket ($!)\n";

$skt->autoflush(1);
print "Connected...\n";
my $sel = new IO::Select( $skt );
print "What is your wish? ";
while (<STDIN>) 
{
    print $skt $_; 
    if ($sel->can_read) {
        print "Received from Server : " . scalar <$skt>; 
    }
    print "What is your wish? ";
}
close $skt or die "Error: Unable to close socket ($!)\n";
