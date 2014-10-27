#!/usr/bin/perl

use Socket;
use Getopt::Std;

$Usage = qq{
Usage: $0 options command...

Options: -d hostname        destination hostname (default: localhost)
         -p port            SVDRP port number (default: 6419)
};

die $Usage if (!$ARGV[0] || !getopts("d:p:"));

$Dest = $opt_d  || "localhost";
$Port = $opt_p  || 6419;
$Cmd  = "@ARGV" || Error("missing command");

$Timeout = 10; # max. seconds to wait for response

$SIG{ALRM} = sub { Error("timeout"); };
alarm($Timeout);

$iaddr = inet_aton($Dest)                   || Error("no host: $Dest");
$paddr = sockaddr_in($Port, $iaddr);

$proto = getprotobyname('tcp');
socket(SOCK, PF_INET, SOCK_STREAM, $proto)  || Error("socket: $!");
connect(SOCK, $paddr)                       || Error("connect: $!");
select(SOCK); $| = 1;
Receive();
Send($Cmd);
Send("quit");
close(SOCK)                                 || Error("close: $!");

sub Send
{
  my $cmd = shift || Error("no command to send");
  print SOCK "$cmd\r\n";
  Receive();
}

sub Receive
{
  while (<SOCK>) {
        print STDOUT $_;
        last if substr($_, 3, 1) ne "-";
        }
}

sub Error
{
  print STDERR "@_\n";
  close(SOCK);
  exit 1;
}
