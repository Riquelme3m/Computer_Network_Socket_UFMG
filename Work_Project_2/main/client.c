#include "Rede.h"
#include "Dispositivo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


void usage(int argc, char **argv);

void request(const char* bT);


#define BUFLEN 500

int local = 0, device = 0, sock;
int mYID;

int main(int argc, char **argv) {
	if (argc < 3) {
		usage(argc, argv);
	}
	char addrstr[100];

	sock = connectToServer(argv[1],argv[2], addrstr);
	if(sock == -1){ usage(argc, argv); }
	//printf("connected to %s\n", addrstr);

	mYID = reqAddCli(sock);
	if(mYID >= 0){
		printf("New ID: %d\n", mYID);
	}
	else{
		exit(0);
	}
	char bTeclado[BUFLEN];
	memset(bTeclado, 0, BUFLEN+1);
	
	while(strcasecmp(bTeclado, "kill") != 0 ){
		memset(bTeclado, 0, BUFLEN+1);
		fgets(bTeclado, BUFLEN, stdin);
		bTeclado[strlen(bTeclado)-1] = 0;
		request(bTeclado);
	}
	close(sock);
	exit(EXIT_SUCCESS);
}


void usage(int argc, char **argv) {
	printf("usage: %s <server IP> <server port>\n", argv[0]);
	printf("example: %s 127.0.0.1 51511\n", argv[0]);
	exit(EXIT_FAILURE);
}


void request(const char* bT){
		int retorno;
		if(strcmp(bT,"show localmaxsensor")==0){
			reqSensorLoc(sock);
		}
		if(strcmp(bT,"show externalmaxsensor")==0){
			reqSensorExt(sock);
		}
		if(strcmp(bT,"show localpotency")==0){
			reqPotLoc(sock);
		}
		if(strcmp(bT,"show externalpotency")==0){
			reqPotExt(sock);
		}
		if(strcmp(bT,"show globalmaxsensor")==0){
			reqSensorGlob(sock);
		}
		if(strcmp(bT,"show globalmaxnetwork")==0){
			reqPotGlob(sock);
		}
		if(strcasecmp(bT,"kill")==0){
			retorno = reqRmCli(sock,mYID);
			if(retorno < 0){
				if(retorno == -4){
					puts("Client not found");
				}
				else{ puts("Erro no código");}
			}
			if(retorno == 1){
				puts("Successful disconnect");
			}
		}
}


