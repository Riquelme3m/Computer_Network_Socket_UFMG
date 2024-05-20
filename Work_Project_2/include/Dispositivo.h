#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>


//--- Parte do servidor ---//
// Inicialização dos sensores
void initSensors();

// Interpretação das requisições dos clientes
int interpretDataClient(const char* msgIn, char* msgOut, int sockServer);

//Interpretação das requisições dos servidores
int interpretDataServer(const char* msgIn, char* msgOut);

//
int getPID();
void setPID(int pid);
//--- Fim da parte do servidor ---//


//--- Parte das requisições do cliente ---//

void reqSensorLoc (int sockCli); // maior sensor local

void reqSensorExt (int sockCli); // maior sensor externo

void reqPotLoc (int sockCli); // potência Útil local

void reqPotExt (int sockCli); // potência Útil externo

void reqSensorGlob (int sockCli);  // maior sensor global

void reqPotGlob (int sockCli); // maior potência Útil global

//--- Fim da parte do cliente ---//
