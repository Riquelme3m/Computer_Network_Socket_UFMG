#pragma once

#include "common.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>

// cria e configura um socket passivo
int createServerSock(char* port, char* addrstr);

// cria e configura um socket ativo do servidor
int acceptConnections(int serverSock, char* clientAddrStr);

// cria um socket no cliente para comunicação com o servidor
int connectToServer(char* adress, char* port, char* addrstr);

// cria o socket e conecta um peer a outro se já houver um peer servidor. 
// Do contrário, o peer se torna um servidor
int connectToPeer(char* adress, 
                  char* port, char* addrstr, 
                  int* sockPass, int* sockAtv);


// inicialização de variáveis que grardam os clientes
void initClis();

//--- Requisições e respostas do Peer ---//

int reqAddPeer(int sock);

int reqRmPeer(int sock, int Id);

int resAddPeer(int sock);

void resRmPeer(int sock, char* id);
//---   ---///


//--- Requisições e respostas do cliente ---//

int reqAddCli(int sock);

int reqRmCli(int sock, int Id);

void resAddCli(int sock);

void resRmCli(int sock, char* id);

//---   ---///

// limpa cliente que se desligou sem mandar msg de requisição
void closeClient(int sock);

// Interpreta mensagens de Rede (controle) do cliente
int interpretNetworkClient(int sock, char* bNet); 

// Interpreta mensagens de Rede (controle) do cliente
int interpretNetworkServer(int sock, char* bNet);

// retorna o socket armazenado do outro Peer
int getOtherSockPeer();
