#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/time.h>


#define PORT 44444

int main()
{       
        int descriptor;                 // File Descriptor
        ssize_t send, read;
        char buffer [1500];             // Nachrichten Buffer
        struct sockaddr_in client;      // Socket Adresse
        socklen_t len = sizeof(client);
        char scan [100];                // Nutzereingabe Nachricht

        fd_set rfds;                    //Variablendeklarationem fuer select()
        struct timeval tv;
        int retval;

        

        descriptor = socket(AF_INET, SOCK_DGRAM, 0);            // UDP Protokoll
        if(descriptor == -1)
                perror("Socketfehler");
        else
                printf("Deskriptor: %i\n", descriptor);

        //memset(&client, '0', len);
        client.sin_family = AF_INET;
        client.sin_addr.s_addr = inet_addr("127.0.0.1");        // Server Adresse
        client.sin_port = htons(PORT);                          // Client an Port binden

        


        scanf("%[^\n]s", scan);                 // Nachrichten Eingabe
        strcpy(buffer, scan);

        //strcpy(buffer, "Hallo Welt");
        //printf("%s gesendet\n", buffer);

        for(int i = 0; i < 5; i++)              // 5mal versuchen Antwort von Server zu bekommen
        {
                FD_ZERO(&rfds);
                FD_SET(descriptor, &rfds);
                tv.tv_sec = 5;                  // Wartezeit in s
                tv.tv_usec = 0;
                
                send = sendto(descriptor, buffer, sizeof(buffer), 0,(struct sockaddr*) &client, len);
                
                retval = select(descriptor+1, &rfds, NULL, NULL, &tv);
                if(retval < 0)
                {
                        perror("Fehler bei select()");
                        break;
                }
                else if(retval > 0)
                {
                        read = recvfrom(descriptor, buffer, sizeof(buffer), 0,(struct sockaddr*) &client, &len);
                        printf("%s erfolgreich an den Server gesendet.\n", buffer);
                        break;
                }    
                else
                        printf("Keine Daten innerhalb von 5 Sekunden empfangen\n");
        }

        return EXIT_SUCCESS;
}

