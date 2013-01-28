#!/bin/bash
if [ $# -ne 4 ]
then
echo "Example: `basename $0` test.com 80 test.com / | telnet"
exit 1
fi
echo "open $1 $2"
sleep 2
echo "GET $4 HTTP/1.1"
echo "Host: $3"
echo "Connection: Keep-Alive"
echo "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.4) Gecko/20070515 Firefox/2.0.0.4"
echo
echo
#sleep 6
#echo "GET $4 HTTP/1.1"
#echo "Connection: Keep-Alive"
#echo "Host: $3"
#echo
#echo
sleep 6
