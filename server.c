#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PORT 44444

int main()
{       
        int descriptor;					// Socket File Descriptor
        ssize_t receive, send;
        char buffer [1500];
        struct sockaddr_in server;		
        socklen_t len = sizeof(server);
        


        descriptor = socket(AF_INET, SOCK_DGRAM, 0);
        if(descriptor == -1)
                perror("Socketfehler");
        else
                printf("Deskriptor: %i\n", descriptor);

        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(PORT);
        bind(descriptor,(struct sockaddr*) &server, len);		// bind Server to address

        fd_set rfds;
        struct timeval tv;
        int retval;


        

        while(1)			// Nachrichten Empfangen dauerhaft
        {

	        FD_ZERO(&rfds);
	        FD_SET(descriptor, &rfds);
	        tv.tv_sec = 5;
	        tv.tv_usec = 0;
	                
            retval = select(descriptor+1, &rfds, NULL, NULL, &tv);
            if(retval < 0)
            {
            	perror("Fehler bei select()");
            }
            else if(retval > 0)
            {
                    receive = recvfrom(descriptor, buffer, sizeof(buffer), 0,(struct sockaddr*) &server, &len);
					if(receive < 0)
						perror("Receive-Fehler\n");
					printf("%s\n", buffer);
					send = sendto(descriptor, buffer, sizeof(buffer), 0,(struct sockaddr*) &server, len);
            }    
        }

        printf("%li", receive);  //wegen -Werror
        printf("%li", send);

        return EXIT_SUCCESS;
}



