//needed for out networking functionality
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

const int LISTEN_PORT_NUMER = 9876;
const int MAX_STRING_SIZE = 255;

int main()
{
     //sockets for the created socket and connections
     int sockfd, newsockfd;
     //length of the client addr
     socklen_t clilen;
     //structures for various information about the client and server
          struct sockaddr_in serv_addr, cli_addr;
     //the message storing strings
     char incoming_message[MAX_STRING_SIZE], temp_string[MAX_STRING_SIZE];

     //Create Socket, if fail, exit from program
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
     {
          printf("Error building Socket FD\n");
          exit(1);
     }
     
     //Set up our server information
          serv_addr.sin_family = AF_INET;
          serv_addr.sin_addr.s_addr = INADDR_ANY;
//          serv_addr.sin_addr.s_addr = htonl(((((192 << 8) | 43) << 8) | 244) << 8) | 18;
//          serv_addr.sin_addr.s_addr = htonl(((((71 << 8) | 204) << 8) | 133) << 8) | 194;
     
          //Assign the port number from assigned port above
          serv_addr.sin_port = htons(LISTEN_PORT_NUMER);
     
     //bind socket 
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     {
          printf("Error in Binding\n");
                   exit(1);
     }
     
     //Set up the socket to listen
        if (listen(sockfd, 2) < 0)
     {
          printf("Error listening\n");
          exit (1);
     };
     
     //infinite loop
     while(1)
     {
          //set the clilen variable prior to passing to accept.
          //per the accept man page (explining why I am doing this)
          //it should initially contain the size of the structure pointed 
          //to by addr; on return it will contain the actual length (in bytes) 
          //of the address returned. This would fail if clilen was null
          clilen = sizeof(cli_addr);
          printf("Ready to accept connections\n");
          newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen );

          if (newsockfd < 0)
          {
               printf("Error accepting connection\n");
                       exit(1);
          }

          printf("A client has connected..\n");

          //read incoming message
          read(newsockfd, incoming_message, sizeof(incoming_message));

          //print received message
          printf("Client said: '%s'\n", incoming_message);


          //create the message to return
          sprintf(temp_string, "Hello %s, you said %s", inet_ntoa(cli_addr.sin_addr), incoming_message);
                    
          //write the return message
          if (write(newsockfd, temp_string, sizeof(incoming_message)) < 0)
          {
               printf("Error writing out\n");
               exit(1);
          }
          printf("Response sent to client\n");
          
          //close the connection
          close (newsockfd);
     }

     //This never executes
     close(sockfd);
     
     return 0;
}
