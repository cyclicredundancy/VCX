#!/usr/bin/perl
use Socket;
use POSIX qw(:sys_wait_h);

$SIG{CHLD} = 'IGNORE';


#socket(SERVER, PF_INET, SOCK_STREAM, getprotobyname('tcp'));
#setsockopt(SERVER, SOL_SOCKET, SO_REUSEADDR, 1);
#$my_addr = sockaddr_in($server_port, INADDR_ANY);
#bind(SERVER, $my_addr)
#    or die "Couldn't bind to port $server_port : $!\n";
#listen(SERVER, SOMAXCONN)
#    or die "Couldn't listen on port $server_port : $!\n";
#print("[$$] STARTED\n");
#while (accept(CLIENT, SERVER)) 
#{
#    next if $pid = fork;
#        die "fork: $!" unless defined $pid;
#    close(SERVER);
#    print("[$$] CONNECTED\n");
#    #print CLIENT "hello\n";
#    while(<CLIENT>)
#    {
#       print("[$$] $_\n");
#       #print CLIENT "hello\n";
#    }
#    print("[$$] EXIT\n");
#    exit;
#} 
#continue 
#{
#    close(CLIENT);
#}
#print("[$$] ENDED\n");
#

use IO::Socket::INET;
use IO::Select;

# flush after every write
$| = 1;

my ($socket,$client_socket);
my ($peer_address,$peer_port);

my $myport = '9876';

# creating object interface of IO::Socket::INET modules which internally does
# socket creation, binding and listening at the specified port address.
#LocalHost => '127.0.0.1',
$socket = new IO::Socket::INET (
LocalHost => 0,
LocalPort => $myport,
Proto => 'tcp',
Listen => SOMAXCONN,
Reuse => 1
) or die "ERROR in Socket Creation : $!\n";

print "SERVER Waiting for client connection on port $myport\n";

while(1)
{
    # waiting for new client connection.
    $client_socket = $socket->accept();

    # get the host and port number of newly connected client.
    $peer_address = $client_socket->peerhost();
    $peer_port = $client_socket->peerport();

    print "Accepted New Client Connection From : $peer_address, $peer_port\n";

    next if $pid = fork;
    die "fork: $!" unless defined $pid;
    $socket->close(); # close listen port in child
    my $sel = new IO::Select( $client_socket );

    $client_socket->autoflush(1);

    while (sysread($client_socket, my $byte, 1) == 1) {
        print "[$$] \'$byte\'\n";
         #$data = "[$$]AsYouWish\n";
         #$client_socket->send($data);
    }

# #    while(<$client_socket>)
#     while(1)
# #    while ($sel->can_read && $client_socket->connected())
# #    while (!$sel->has_exception && $sel->can_read)
#     {
#         # read operation on the newly accepted client
#         #$data = <$client_socket>;
#         #$data = $_;
# 
#         # we can also read from socket through recv()  in IO::Socket::INET
#         break unless $client_socket->recv($data,1024);
#         print "[$$] Received from Client : $data";
# 
# 
#         # write operation on the newly accepted client.
#         $data = "[$$]AsYouWish\n";
# 
#         #print $client_socket $data;
#         # we can also send the data through IO::Socket::INET module,
#         $client_socket->send($data);
#         
#         break unless $client_socket;
# 
#     }
    print "[$$] Client left\n";
    $client_socket->close();
    exit;
}

$socket->close();

