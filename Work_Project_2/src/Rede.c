#include "Rede.h"
//#ifdef SERVER_SIDE

// dados basicos dos clientes
typedef struct Client{
	int sock;
	int id; 
}Client;


// Lembrar de guardar o outro ID do servidor aqui
int numPeers, otherSockPeer = -1, otherID = -1;
int numCli;



Client clis[10];
const Client zero = {-1, -1};


void initClis(){
	for(int i = 0; i < 10; i++){
		clis[i] = zero;
	}
}

int getOtherSockPeer(){return otherSockPeer;}

// constantes dos valores de erro e de acordo com o roteiro
enum erro {cliLimitExceeded=1,peerLimitExceeded,peerNotFound,cliNotFound};


// Tipos de mensagens de controle
const char *typesControl[6] = {"REQ_ADDPEER", "REQ_DCPEER", "RES_ADDPEER",
							   							 "REQ_ADD", 	  "REQ_DC", 	"RES_ADD"};


int connectToServer(char* adress, char* port, char* addrstr){
	struct sockaddr_storage storage;
	if (0 != addrparse(adress, port, &storage)) {
		return -1;
	}

	int sockServer;
	sockServer = socket(storage.ss_family, SOCK_STREAM, 0); // cria o socket
	if (sockServer == -1) {
		logexit("socket");
	}
	struct sockaddr *addr = (struct sockaddr *)(&storage);
	
	// conecta o socket ao servidor
	if (0 != connect(sockServer, addr, sizeof(storage))) {
		logexit("connect");
	}
	addrtostr(addr, addrstr, 100);
	return sockServer;
}

int createServerSock(char* port, char* addrstr){
	struct sockaddr_storage storage;
	if (0 != server_sockaddr_init("v4", port, &storage)) {
			return -1;
	}

	int severSock;
	severSock = socket(storage.ss_family, SOCK_STREAM, 0); // criando  o socket
	if (severSock == -1) {
			logexit("socket");
	}

	int enable = 1;
	// colocando o socket para reusar o endereço
	if (0 != setsockopt(severSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
			logexit("setsockopt");
	}
	
	// ligando o socket ao endereço
	struct sockaddr *addr = (struct sockaddr *)(&storage);
	if (0 != bind(severSock, addr, sizeof(storage))) {
			logexit("bind");
	}

	// colocando o socket no modo passivo para aceitar novas conexões
	if (0 != listen(severSock, 10)) {
			logexit("listen");
	}
	addrtostr(addr, addrstr, 100);
	return severSock;
}


int acceptConnections(int serverSock, char* clientAddrStr){
	struct sockaddr_storage cstorage;
	struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
	socklen_t caddrlen = sizeof(cstorage);

	// aceitando uma nova conexão
	int csock = accept(serverSock, caddr, &caddrlen);
	if (csock == -1) {
			logexit("accept");
	}
	addrtostr(caddr, clientAddrStr, 100);
	return csock;
}


int connectToPeer(char* adress, char* port, char* addrstr, int* sockPass, int* sockAtv){
	struct sockaddr_storage storage;
	if (0 != addrparse(adress, port, &storage)) {
		return -1;
	}

	*sockAtv = socket(storage.ss_family, SOCK_STREAM, 0); // cria o socket
	if (*sockAtv == -1) {
		logexit("socket");
	}
	struct sockaddr *addr = (struct sockaddr *)(&storage);
	
	// conecta o socket ao servidor
	if (0 != connect(*sockAtv, addr, sizeof(storage))) {
		printf("Erro: %s\n", strerror(errno));
		puts("No peer found, starting to listen..");
		*sockAtv = -1;
		*sockPass = createServerSock(port, addrstr);
		numPeers = 1;
		return 0;
	}
	else{
		addrtostr(addr, addrstr, 100);
		numPeers = 2;
		return reqAddPeer(*sockAtv);
	}
}



void closeClient(int sock){
	for(int i = 0; i < 10; i++){
		if(clis[i].sock == sock) {clis[i] = zero;}
	}
}




//--- Funções de requisição ---/

int reqAddPeer(int sock){
	char buff[500], *ptr;
	int aux = 0;
	memset(buff,0,500);
	aux = send(sock,typesControl[0], strlen(typesControl[0]),0);
	aux = recv(sock,buff,500,0);
	ptr = strtok(buff,"()");
	if(strcmp(ptr,"ERROR") == 0){
		ptr = strtok(NULL,"()");
		if(atoi(ptr) == 2)
			return -2;
		else
			return -5;
	}
	else if(strcmp(ptr,typesControl[2]) == 0){
		int myPID, otherPID;
		ptr = strtok(NULL,"()");
		myPID = atoi(ptr);
		printf("New Peer ID: %d\n", myPID);

		do{
			otherPID = 20 + rand()%10; // entre 20 e 29
		}while(myPID == otherPID);
		sprintf(buff, "%s(%d)", typesControl[2], otherPID);
		printf("Peer %d connected\n", otherPID);
		aux = send(sock,buff, strlen(buff),0);
		return myPID;
	}
	else{
		return -5;
	}
}


int reqRmPeer(int sock, int Id){
	char buffIn[500], buffOut[500], *ptr;
	int status;
	memset(buffIn,0,500);
	memset(buffOut,0,500);
	sprintf(buffOut, "%s(%d)", typesControl[1], Id);
	status = send(sock,buffOut, strlen(buffOut),0);
	status = recv(sock,buffIn,500,0);
	ptr = strtok(buffIn,"()");
	if(strcmp(ptr,"ERROR") == 0){
		ptr = strtok(NULL,"()");
		if(atoi(ptr) == 3)
			return -3;
		else
			return -5;
	}
	else if(strcmp(ptr,"OK") == 0){
		int code;
		ptr = strtok(NULL,"()");
		code = atoi(ptr);
		if(code == 1){
			return 1;
		}
		else{
			return -5;
		}
	}
	else{
		return -5;
	}
}



int reqAddCli(int sock){
	char buff[500], *ptr;
	memset(buff,0,500);
	send(sock,typesControl[0+3], strlen(typesControl[0+3]),0);
	recv(sock,buff,500,0);
	ptr = strtok(buff,"()");
	if(strcmp(ptr,"ERROR") == 0){
		ptr = strtok(NULL,"()");
		if(atoi(ptr) == 1)
			return -1;
		else
			return -5;
	}
	else if(strcmp(ptr,typesControl[2+3]) == 0){
		ptr = strtok(NULL,"()");
		return atoi(ptr);
	}
	else{
		return -5;
	}
}


int reqRmCli(int sock, int Id){
	char buffIn[500], buffOut[500], *ptr;
	int status;
	memset(buffIn,0,500);
	memset(buffOut,0,500);
	sprintf(buffOut, "%s(%d)", typesControl[1+3], Id);
	status = send(sock,buffOut, strlen(buffOut),0);
	status = recv(sock,buffIn,500,0);
	ptr = strtok(buffIn,"()");
	if(strcmp(ptr,"ERROR") == 0){
		ptr = strtok(NULL,"()");
		if(atoi(ptr) == 4)
			return -4;
		else
			return -5;
	}
	else if(strcmp(ptr,"OK") == 0){
		int code;
		ptr = strtok(NULL,"()");
		code = atoi(ptr);
		if(code == 1){
			return 1;
		}
		else{
			return -5;
		}
	}
	else{
		return -5;
	}
}

//--- Fim de funções de requisição ---/

//--- Funções de resposta ---/

int resAddPeer(int sock){
	char buffOut[500], buffIn[500];
	memset(buffIn,0,500); // limpando buffer de entrada
	if(numCli < 2){
		char *ptr;
		int aux = 0, myID;
		otherSockPeer = sock;
		otherID = 20 + rand()%10;
		sprintf(buffOut, "%s(%d)",typesControl[2], otherID);
		printf("Peer %d added\n", otherID);
		aux = send(sock,buffOut, strlen(buffOut),0);
		if(aux == -1){
			printf("Erro: %s\n", strerror(errno));
		}
		aux = recv(sock,buffIn, 500,0);
		ptr = strtok(buffIn,"()");
		if(strcmp(ptr,typesControl[2])!=0){
			return 1;
		}
		ptr = strtok(NULL,"()");
		myID = atoi(ptr);
		printf("New Peer ID: %d\n", myID);
		return myID;
	}
	else{
		int aux = 0;
		sprintf(buffOut, "ERROR(0%d)",peerLimitExceeded);
		aux = send(sock,buffOut, strlen(buffOut),0);
		return 0;
	}
}


void resAddCli(int sock){
	char buffOut[500];
	if(numCli < 10){
		int i = 0;
		while(clis[i].id>0){i++;}
		clis[i].sock = sock;
		clis[i].id = i;
		sprintf(buffOut, "%s(%d)",typesControl[2+3], i);
		printf("Client %d added\n", i);
	}
	else{
		sprintf(buffOut, "ERROR(0%d)",cliLimitExceeded);
	}
	send(sock,buffOut, strlen(buffOut),0);
}


void resRmPeer(int sock, char* id){
	char buffOut[500];
	int idNumber = atoi(id);
	if(otherID == idNumber && otherSockPeer == sock){
		otherID = -1;
		otherSockPeer = -1;
		sprintf(buffOut, "OK(01)");
	}
	else{
		sprintf(buffOut, "ERROR(0%d)",peerNotFound);
	}
	send(sock,buffOut, strlen(buffOut),0);
}


void resRmCli(int sock, char* id){
	char buffOut[500];
	int idNumber = atoi(id);
	if(clis[idNumber].id == idNumber){
		clis[idNumber].sock = -1;
		clis[idNumber].id = -1;
		sprintf(buffOut, "OK(01)");
	}
	else{
		sprintf(buffOut, "ERROR(0%d)",cliNotFound);
	}
	printf("Client %d removed\n", idNumber);
	send(sock,buffOut, strlen(buffOut),0);
}

//--- Fim funções de resposta ---/

int interpretNetworkClient(int sock, char* bNet){		
	char aux[20], *ptr;
	strcpy(aux,bNet);
	ptr = strtok(aux,"()");
	if(strcmp(bNet,typesControl[0+3])==0){ //REQ_ADD
		resAddCli(sock);
	}
	else if(strncmp(bNet,typesControl[1+3],strlen(typesControl[1+3]))==0){ //REQ_DC
		ptr = strtok(NULL,"()");
		resRmCli(sock, ptr);
	}
	else{
		return 1;
	}
	return 0;
}

int interpretNetworkServer(int sock, char* bNet){		
	char aux[20], *ptr;
	strcpy(aux,bNet);
	ptr = strtok(aux,"()");
	if(strcmp(bNet,typesControl[0])==0){// REQ_ADDPEER
		return resAddPeer(sock);
	}
	else if(strncmp(bNet,typesControl[1],strlen(typesControl[1]))==0){// REQ_DCPEER
		ptr = strtok(NULL,"()");
		resRmPeer(sock, ptr);
	}
	else{
		return -1;
	}
	return 0;
}

