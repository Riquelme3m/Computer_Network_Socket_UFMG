#include "Rede.h"
#include "Dispositivo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h> 
//#include <sys/select.h>


#define BUFLEN 500

void usage(int argc, char **argv);


char *debug;

int main(int argc, char **argv) {
    if (argc < 4) {
        usage(argc, argv);
    }

    char addrstr[100], clientAddrStr[100], bIn[BUFLEN], bOut[BUFLEN];
    
		int serverPasSocker, serverAtvSocker[100], numPeers = 0;
    int clientPasSocker, clientAtvSocker[100], numClis = 0;
    int status;

		// select
		fd_set readfds; 
		int new_socket, activity , valread , sd1, sd2; 
		int max_sd; 
		srand(time(0));
		// inicializando os socks ativos
		for(int i = 0; i < 100; i++){
			serverAtvSocker[i] = -1;
			clientAtvSocker[i] = -1;
		}
		initClis();
		initSensors();

    status = connectToPeer(argv[1],argv[2],addrstr, &serverPasSocker, &serverAtvSocker[0]);
		if(status < 0){
			close(serverAtvSocker[0]);
			exit(0);
		}
		setPID(status);
    //printf("bound to %s, waiting connections\n", addrstr);
		clientPasSocker = createServerSock(argv[3],addrstr);
		while(1) 
	{ 
		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(clientPasSocker, &readfds); 
		max_sd = clientPasSocker;

		if(serverPasSocker > 0){
			FD_SET(serverPasSocker, &readfds); 
			if(serverPasSocker > max_sd) {max_sd = serverPasSocker; }
		}
			
		//add child sockets to set 
		for ( int i = 0 ; i < 100 ; i++) 
		{ 
			//socket descriptor 
			sd1 = serverAtvSocker[i]; 
			sd2 = clientAtvSocker[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd1 > 0){
				FD_SET( sd1 , &readfds);
				if(sd1 > max_sd) {max_sd = sd1; }
			}
			if(sd2 > 0){
				FD_SET( sd2 , &readfds);
				if(sd2 > max_sd) {max_sd = sd2; }
			}
		} 

		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		//If something happened on the passive socket , 
		//then its an incoming connection 
		if (FD_ISSET(clientPasSocker, &readfds)) { 
			new_socket = acceptConnections(clientPasSocker, clientAddrStr);
			if(numClis < 100){
				for (int i = 0; i < 100; i++) { 
					if( clientAtvSocker[i] < 0 ) { 
						clientAtvSocker[i] = new_socket;							
						break; 
					} 
				} 
			}
			else{
				close(new_socket);
			}
		} 

		if(serverPasSocker>0){
		if (FD_ISSET(serverPasSocker, &readfds)) { 
			new_socket = acceptConnections(serverPasSocker, clientAddrStr);
			if(numPeers < 100){
				//add new socket to array of sockets 
				for (int i = 0; i < 100; i++) 
				{ 
					//if position is empty 
					if( serverAtvSocker[i] < 0 ) 
					{ 
						serverAtvSocker[i] = new_socket;							
						break; 
					} 
				} 
			}
			else{
				close(new_socket);
			}
		} 
		}
		//else its some IO operation on some other socket 
		for (int i = 0; i < 100; i++) 
		{ 
			sd1 = serverAtvSocker[i]; 
			sd2 = clientAtvSocker[i]; 

			// Verificando dados de Peers
			if (sd1 > 0 && FD_ISSET( sd1 , &readfds)) 
			{ 
				memset(bIn, 0, 500);
				if ((valread = recv( sd1 , bIn, 500,0)) == 0) 
				{ 
					close( sd1 ); 
					serverAtvSocker[i] = -1; 
				} 
				else
				{ 
					if(interpretDataServer(bIn,bOut)<0){ // ou seja, não é requisição de dados
						// requisição de conexão
						int aux = interpretNetworkServer(sd1, bIn);
						if(aux < 0){
							puts("Erro no código 01");
						}
						else if(aux > 0){
							setPID(aux);
						}
					}
						else{
							send(sd1 , bOut , strlen(bOut) , 0 ); 
						}
					}
			} 

			// Verificando dados de clientes
			if (sd2 > 0 && FD_ISSET( sd2 , &readfds)) 
			{ 
				memset(bIn, 0, 500);
				if ((valread = recv( sd2 , bIn, 500,0)) == 0) 
				{ 
					closeClient(sd2);
					close( sd2 ); 
					clientAtvSocker[i] = -1; 
				} 
				else
				{ 
					if(interpretDataClient(bIn,bOut, getOtherSockPeer())<0){ // ou seja, não é requisição de dados
						// requisição de conexão
						if(interpretNetworkClient(sd2, bIn) < 0){
							puts("Erro no código 02");
						}
					}
					else{
						send(sd2 , bOut , strlen(bOut) , 0 ); 
					}
				} 
			}
		} 
	} 

		close(serverPasSocker);
    exit(EXIT_SUCCESS);
}


void usage(int argc, char **argv){
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}


// 