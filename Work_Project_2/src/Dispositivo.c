#include "Dispositivo.h"

// Inconsistência entre os valores de obtenção de 
// potência útil externa do roteiro com o do exemplo
// de execução

// dados basicos dos sensores
typedef struct sensor{
	int id;
	int power; 
	int efficiency;
}sensor;

// Tipos de mensagens de dados
const char *typesData[12] = {"REQ_LS", "REQ_ES", "REQ_LP", "REQ_EP", "REQ_MS", "REQ_MN", 
							 "RES_LS", "RES_ES", "RES_LP", "RES_EP", "RES_MS", "RES_MN", };

// lista de sensores
sensor sensors[10];

// Pid do servidor
int PidM = 0;

int getPID(){
	return PidM;
}

void setPID(int pid){
	PidM = pid;
}

void initSensors(){
	for(int i = 0; i < 10; i++){
		sensors[i].id = i + 1;
		sensors[i].power = rand()%1501;
		sensors[i].efficiency = rand()%101;
	}
}


sensor* consultMaxLoc(){
	int max = 0;
	int potmax = sensors[max].power*sensors[max].efficiency;
	for(int i = 1; i < 10; i++){
		if(sensors[i].power*sensors[i].efficiency > potmax){
			max = i;
			potmax = sensors[max].power*sensors[max].efficiency;
		}
	}
	return &sensors[max];
}


double consultPotUsefulLoc (){
	double potUseful = 0;
	for(int i = 1; i < 10; i++){
		potUseful += sensors[i].power*sensors[i].efficiency;
	}
	return potUseful/100;
}

//---- Início parte do servidor
int interpretDataClient(const char* msgIn, char* msgOut, int sockServer){
	sensor *eq;

	if(strcmp(msgIn,typesData[0]) == 0){ //REQ_LS
		eq = consultMaxLoc();
		sprintf(msgOut,"%s(%d %d: %.2f (%d %d))", typesData[0+6], PidM , eq->id, ((double)(eq->efficiency*eq->power))/100, eq->power, eq->efficiency);
		return 0;
	}

	else if(strcmp(msgIn,typesData[1]) == 0){ //REQ_ES
		int status = send(sockServer,msgIn, strlen(msgIn),0);
		status = recv(sockServer,msgOut,500,0);
		return 0;
	}

	else if(strcmp(msgIn,typesData[2]) == 0){ //REQ_LP
		sprintf(msgOut,"%s(%d %.2f)", typesData[2+6], PidM, consultPotUsefulLoc());
		return 0;
	}

	else if(strcmp(msgIn,typesData[3]) == 0){ //REQ_EP
		int status = send(sockServer,msgIn, strlen(msgIn),0);
		status = recv(sockServer,msgOut,500,0);
		return 0;
	}

	else if(strcmp(msgIn,typesData[4]) == 0){ //REQ_MS
		char internMsg[500], number[10], *values[2];
		double locSensor, extSensor;
		
		// enviando a requisição ao outro Peer e recebendo a resposta
		memset(internMsg,0,500);
		memset(number,0,10);
		int status = send(sockServer,typesData[1], strlen(typesData[1]),0);
		status = recv(sockServer,internMsg,500,0);
		// obtém o sensor do outro servidor com a maior potencia 
		if(strncmp(internMsg, typesData[1+6], strlen(typesData[1+6])) != 0){
			return 1;
		}
		values[0] = strchr(internMsg, ':');
		values[1] = strchr(values[0], '(');
		strncpy(number,values[0]+2, values[1] - values[0] - 3);
		locSensor = ((double)(eq->efficiency*eq->power))/100;

		// obtém o sensor com a maior potencia localmente e compara com o externo
		eq = consultMaxLoc();
		extSensor = strtod(number, NULL);
		if(locSensor >= extSensor){
			sprintf(msgOut,"%s(%d %d: %.2f (%d %d))", typesData[4+6], PidM , eq->id, locSensor, eq->power, eq->efficiency);
		}
		else{
			internMsg[4] = 'M';
			strcpy(msgOut, internMsg);
		}
		return 0;
	}

	else if(strcmp(msgIn,typesData[5]) == 0){ //REQ_MN
		char internMsg[500], number[10], *values[2];
		// enviando a requisição ao outro Peer e recebendo a resposta
		memset(internMsg,0,500);
		int status = send(sockServer,typesData[3], strlen(typesData[3]),0);
		status = recv(sockServer,internMsg,500,0);
		
		// obtém a potência útil do outro peer e compara com a potência util local 
		if(strncmp(internMsg, typesData[3+6], strlen(typesData[3+6])) != 0){
			return 1;
		}
		values[0] = strchr(internMsg, ' ');
		values[1] = strchr(values[0], ')');
		strncpy(number,values[0]+1, values[1] - values[0] - 2);
		if(consultPotUsefulLoc() >= strtod(number, NULL)){
			sprintf(msgOut,"%s(%d %.2f)", typesData[5+6], PidM, consultPotUsefulLoc());
		}
		else{
			// trocando somente as letras que diferem a resposta 
			// do servidor corrente com a recebida pelo outro peer
			internMsg[4] = 'M';
			internMsg[5] = 'N';
			strcpy(msgOut, internMsg);
		}
		return 0;
	}

	else{
		return -1;
	}
}


int interpretDataServer(const char* msgIn, char* msgOut){
	sensor *eq;

	if(strcmp(msgIn,typesData[1]) == 0){ // REQ_ES
		eq = consultMaxLoc();
		sprintf(msgOut,"%s(%d %d: %.2f (%d %d))", typesData[1+6], PidM , eq->id, ((double)(eq->efficiency*eq->power))/100, eq->power, eq->efficiency);
		return 0;
	}


	else if(strcmp(msgIn,typesData[3]) == 0){ // REQ_LP
		sprintf(msgOut,"%s(%d %.2f)", typesData[3+6], PidM, consultPotUsefulLoc());
		return 0;
	}

	else{
		return -1;
	}
}

//---- Fim parte do servidor ----//

//---- Início cliente ----/
void reqSensorLoc (int sockCli){
	int status;
	char bIn[500], *ptr;
	memset(bIn,0,500);
	status = send(sockCli, typesData[0], strlen(typesData[0]),0);
	status = recv(sockCli, bIn, 500, 0);
	ptr = strtok(bIn,"(");
	if(strcmp(ptr,typesData[0+6])==0){
		ptr = strtok(NULL, " ");
		printf("local %s sensor ", ptr);
		ptr = strtok(NULL, ";");
		puts(ptr);
	}
	else{
		puts("erro no reqSensor_Loc");
	}
}


void reqSensorExt (int sockCli){
	int status;
	char bIn[500], *ptr;
	memset(bIn,0,500);
	status = send(sockCli, typesData[1], strlen(typesData[1]),0);
	status = recv(sockCli, bIn, 500, 0);
	ptr = strtok(bIn,"(");
	if(strcmp(ptr,typesData[1+6])==0){
		ptr = strtok(NULL, " ");
		printf("external %s sensor ", ptr);
		ptr = strtok(NULL, ")");
		puts(ptr);
	}
	else{
		puts("erro no Ext");
	}
}


void reqPotLoc (int sockCli){
	int status;
	char bIn[500], *ptr;
	memset(bIn,0,500);
	status = send(sockCli, typesData[2], strlen(typesData[2]),0);
	status = recv(sockCli, bIn, 500, 0);
	ptr = strtok(bIn,"(");
	if(strcmp(ptr,typesData[2+6])==0){
		ptr = strtok(NULL, " ");
		printf("local %s potency: ", ptr);
		ptr = strtok(NULL, ")");
		puts(ptr);
	}
	else{
		puts("erro no reqPotLoc");
	}
}


void reqPotExt (int sockCli){
	int status;
	char bIn[500], *ptr;
	memset(bIn,0,500);
	status = send(sockCli, typesData[3], strlen(typesData[3]),0);
	status = recv(sockCli, bIn, 500, 0);
	ptr = strtok(bIn,"(");
	if(strcmp(ptr,typesData[3+6])==0){
		ptr = strtok(NULL, " ");
		printf("external %s potency: ", ptr);
		ptr = strtok(NULL, ")");
		puts(ptr);
	}
	else{
		puts("erro no reqPotExt");
	}
}


void reqSensorGlob (int sockCli){
	int status;
	char bIn[500], *ptr;
	memset(bIn,0,500);
	status = send(sockCli, typesData[4], strlen(typesData[4]),0);
	status = recv(sockCli, bIn, 500, 0);
	ptr = strtok(bIn,"(");
	if(strcmp(ptr,typesData[4+6])==0){
		ptr = strtok(NULL, " ");
		printf("global %s sensor ", ptr);
		ptr = strtok(NULL, ")");
		puts(ptr);
	}
	else{
		puts("erro no reqSensorGlob");
	}
}


void reqPotGlob (int sockCli){
	int status;
	char bIn[500], *ptr;
	memset(bIn,0,500);
	status = send(sockCli, typesData[5], strlen(typesData[5]),0);
	status = recv(sockCli, bIn, 500, 0);
	ptr = strtok(bIn,"(");
	if(strcmp(ptr,typesData[5+6])==0){
		ptr = strtok(NULL, " ");
		printf("global %s potency: ", ptr);
		ptr = strtok(NULL, ")");
		puts(ptr);
	}
	else{
		puts("erro no reqPotGlob");
	}
}

//---- Fim cliente ----/


