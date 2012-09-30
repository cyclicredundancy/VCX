//needed for out networking functionality
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
     //for the socket file descriptor, portnumber, and return status
     int sockfd, portno, n;
     //buffer for the messages
     char buffer[255];

     //server information and structure for host info returned by gethostbyname
     struct sockaddr_in serv_addr;
         struct hostent *server;

     //I hate using a union for this purpose, but I had issues with copying and casting
     union sock
     {
          struct     sockaddr s;
          struct  sockaddr_in i;
     } sock;
     
     //not exactly the best way to validate commandline input, but it will do for demonstration
     if (argc < 4)
     {
          printf("Please provide correct arguments\n");
          exit(1);
     }
     
     //Get the port number from the command line
     portno = atoi(argv[2]);
     
     //set up the socket file descriptor
         sockfd = socket(AF_INET, SOCK_STREAM, 0);
         if (sockfd < 0) 
             exit(1);
     
     //get server from argument passed into the command line
     server = gethostbyname(argv[1]); 
     if (server == NULL) {
             fprintf(stderr,"ERROR, no such host\n");
             exit(1);
         }
     
     //Clear out the servers info
     bzero((char *) &serv_addr, sizeof(serv_addr));
     
     //Set to a network socket instead of a Unix Domain
     serv_addr.sin_family = AF_INET;

     //byte copy the address retrieved from server  into the
     //server addr structure
     bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
     serv_addr.sin_port = htons(portno);

     //Copy the serv_addr into the union
     bcopy(&serv_addr, &sock.i, sizeof(struct sockaddr_in));

     //Connect to the server
     if ( connect(sockfd, &sock.s, sizeof(struct sockaddr)) < 0)
     {
          printf("Error with connection\n");
             exit(1);
     }
     
     //Show the user what they put intot he command line
     printf("Message typed in is: '%s' \n", argv[3]);
     
     //write to the server
     write(sockfd, argv[3], sizeof(buffer));

     //Read data back from server
     read(sockfd, buffer, sizeof(buffer));
     
     //Show the user what was recieved from the server
     printf("Recieved from server: '%s'\n", buffer);

     //Close the socket
     close(sockfd);

     return 0;
}
