#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#define BUFSZ 500

void usage(int argc, char **argv) {
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage)) {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }

    int enable = 1;
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage))) {
        logexit("bind");
    }

    if (0 != listen(s, 10)) {
        logexit("listen");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound to %s, waiting connections\n", addrstr);



    int valor1,valor2,valor3,valor4;
            int potencia;
            int linhas=100;
            int colunas=5;
            int matriz[linhas][colunas];
             for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
            matriz[i][j] = -1;
        }
    }
            




    while (1) {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1) {
            logexit("accept");
        }

        char caddrstr[BUFSZ];
        addrtostr(caddr, caddrstr, BUFSZ);
        printf("[log] connection from %s\n", caddrstr);
//---------------------------------------------------------------------------------
        
        /*int valor1,valor2,valor3,valor4;
            int potencia;
            int linhas=100;
            int colunas=5;
            int matriz[linhas][colunas];*/


            char buf[BUFSZ];
            size_t count,count1;
            //int sensorexiste=0;



        
        while (1) {
            start:

            
            memset(buf, 0, BUFSZ);
           int sensorexiste=0;
            //size_t count = recv(csock, buf, BUFSZ - 1, 0);
            count = recv(csock, buf, BUFSZ - 1, 0); //aqui eu recebo os dados do cliente
            
            if (count == 0) {
                // A conexão foi encerrada pelo cliente.
                //printf("codigo no if count");
                break;
            }

            //printf("[msg] %s, %d bytes: %s\n", caddrstr, (int)count, buf);
            //printf("%s\n", buf);

            //---------------------TRATANDO OS DADOS QUE RECEBO DO SERVIDOR , MANIPULANDO O NOSSO BANCO DE DADOS E ENVIANDO AS DEVIDAS
            // RESPOSTAS PARA O CLIENTE

            if (strcmp(buf, "kill") == 0){
            //puts(buf);
            char mensagemerro[BUFSZ];
            snprintf(mensagemerro,BUFSZ,"kill");
            count = send(csock, mensagemerro, strlen(mensagemerro) + 1, 0);
            close(csock);
            exit(EXIT_SUCCESS);
            break;
            }

            //--------------------------------------------------------------------

            //instalando sensores e enviando as mensagem devidas para o cliente

            if (sscanf(buf, "INS_REQ %d %d %d %d", &valor1, &valor2, &valor3, &valor4) == 4){
            puts(buf);
            
            //printf("%d %d %d %d \n", valor1,valor2,valor3,valor4);//so teste
           
            for (int i = 0; i < linhas; i++) {
            
                if((valor1==matriz[i][0]) ){
                    //MANDA UMA MENSAGEM PARA O CLIENTE COM O SEGUINTE ERRO : sensor already exists
                    sensorexiste=1;
                    /*int a,b,c,d;
                    a=matriz[i][0];
                    b=matriz[i][1];
                    c=matriz[i][2];
                    d=matriz[i][3];
                    printf("dados %d %d %d %d \n",a,b,c,d);*/
                char mensagemerro[BUFSZ];
                snprintf(mensagemerro,BUFSZ,"sensor already exists");
            count = send(csock, mensagemerro, strlen(mensagemerro) + 1, 0);
            if (count != strlen(mensagemerro) + 1) {
                logexit("send");
            }
            goto start;
            //close(csock);


                }
        
    }
            if(sensorexiste!=1){
                potencia=valor2*valor3; //calculei a potência e adicionei o sensor na tabela
                //agora eu adiciono o sensor na tabela pq eu já sei que ele não existe
                //printf("%d\n",potencia);

               
                for(int i=0;i<linhas;i++){
                    
                    if((matriz[i][0]==-1 )&& (matriz[i][1]==-1)){
                        matriz[i][0]=valor1;
                        matriz[i][1]=valor2;
                        matriz[i][2]=valor3;
                        matriz[i][3]=valor4;
                        matriz[i][4]=potencia;
                        //printf("CÓDIGO ENTROU NO IF \n");
                        //armazenei todos os valores em uma matriz
                        //sairdoloop=1;
                        break;
                    }
                    /*if(sairdoloop){
                        break;
                    }*/
                    
                }
                    char mensagemsucesso[BUFSZ];
                    snprintf(mensagemsucesso,BUFSZ,"successfull installation");

                    count1 = send(csock, mensagemsucesso, strlen(mensagemsucesso) + 1, 0);
                    goto start;
                    
                    //printf("sucessfull enviado para o cliente");
                
                    /*if (count1 != strlen(mensagemsucesso) + 1) {
                    logexit("send");
                    }*/
                
                 //close(csock);
                        
                    }
                    //break;
                }



            //-------------------------------------------------------------
            //REMOVENDO SENSORES DA ESTRUTURA DE DADOS
            



            if (sscanf(buf, "REM_REQ %d", &valor1) == 1) {
            puts(buf);
            //printf("%d \n", valor1);//so teste
            for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if(matriz[i][0]==valor1){
                    
                    sensorexiste=1;
                    matriz[i][0]=-1;
                    matriz[i][1]=-1;
                    matriz[i][2]=-1;
                    matriz[i][3]=-1;
                    matriz[i][4]=-1;


                char mensagemerro2[BUFSZ];
                snprintf(mensagemerro2,BUFSZ,"successful removal");
            count = send(csock, mensagemerro2, strlen(mensagemerro2) + 1, 0);
            if (count != strlen(mensagemerro2) + 1) {
                logexit("send");
            }
            
            goto start;
            
            //close(csock);
            }
            }
        
    }
            if(sensorexiste==0){
                
                    char mensagemsucesso[BUFSZ];
                    snprintf(mensagemsucesso,BUFSZ,"sensor not installed");

                    count = send(csock, mensagemsucesso, strlen(mensagemsucesso) + 1, 0);
                    //printf("sucessfull enviado para o cliente");
                
                    if (count != strlen(mensagemsucesso) + 1) {
                    logexit("send"); 
                    }
                    
                goto start;
                 //close(csock);
                        
                    }
                //break;
            }
          
            //tratando para atualizar informações dos sensores

            if (sscanf(buf, "CH_REQ %d %d %d %d", &valor1, &valor2, &valor3, &valor4) == 4){
                puts(buf);
                
                //verificar se o sensor existem na tabela

                for (int i = 0; i < linhas; i++) {
            
                if((valor1==matriz[i][0]) ){
                    //MANDA UMA MENSAGEM PARA O CLIENTE COM O SEGUINTE ERRO : sensor already exists
                    sensorexiste=1;
                    potencia=valor2*valor3;
                    matriz[i][1]=valor2;
                    matriz[i][2]=valor3;
                    matriz[i][3]=valor4;
                    matriz[i][4]=potencia;

                char mensagemerro[BUFSZ];
                snprintf(mensagemerro,BUFSZ,"successful change");
            count = send(csock, mensagemerro, strlen(mensagemerro) + 1, 0);
            if (count != strlen(mensagemerro) + 1) {
                logexit("send");
            }
            goto start;
            //close(csock);
                }
    }

    if(sensorexiste==0){
                
                    char mensagemsucesso[BUFSZ];
                    snprintf(mensagemsucesso,BUFSZ,"sensor not installed");

                    count = send(csock, mensagemsucesso, strlen(mensagemsucesso) + 1, 0);
                    //printf("sucessfull enviado para o cliente");
                
                    if (count != strlen(mensagemsucesso) + 1) {
                    logexit("send"); 
                    }
                    
                goto start;
                 //close(csock);
                        
                    }

















            }


        if (sscanf(buf, "SEN_REQ %d", &valor1) == 1){
            puts(buf);

            for(int i=0;i<linhas;i++){
                if(matriz[i][0]==valor1){
                    sensorexiste=1;

                char mensagemerro[BUFSZ];
                snprintf(mensagemerro,BUFSZ,"SEN_RES %d %d %d",matriz[i][0],matriz[i][3],matriz[i][4]);
                count = send(csock, mensagemerro, strlen(mensagemerro) + 1, 0);
                if (count != strlen(mensagemerro) + 1) {
                logexit("send");
                 }
                goto start;

                }
            }
             if(sensorexiste==0){
                
                    char mensagemsucesso[BUFSZ];
                    snprintf(mensagemsucesso,BUFSZ,"sensor not installed");

                    count = send(csock, mensagemsucesso, strlen(mensagemsucesso) + 1, 0);
                    //printf("sucessfull enviado para o cliente");
                
                    if (count != strlen(mensagemsucesso) + 1) {
                    logexit("send"); 
                    }
                    
                goto start;
                 //close(csock);
                        
                    }


        }


        if (strcmp(buf, "VAL_REQ") == 0){
            puts(buf);
            //verificar se existe sensor , se não existe mandar para o cliente "no sensors";
            //se existe eu mando o VAL_RES 0 1 2 3 4 5 6...
            int index=8;
            for(int i=0;i<linhas;i++){
                if(matriz[i][0]!=-1){
                    sensorexiste=1;
                }
            }
            if(sensorexiste==1){
                char mensagemenviar[BUFSZ];
                int index = snprintf(mensagemenviar, BUFSZ, "VAL_RES");

                for(int i=0;i<linhas;i++){
                    if(matriz[i][0]==-1){
                        continue;
                    }
                    else { 
                    for(int j=0;j<colunas;j++){
                        
                        
                        
                        index += snprintf(mensagemenviar + index, BUFSZ - index, " %d %d %d",
                             matriz[i][0], matriz[i][4], matriz[i][3]);
                             break;

                    }
                    } 

                }

                
                
                
                count = send(csock, mensagemenviar, strlen(mensagemenviar) + 1, 0);
                if (count != strlen(mensagemenviar) + 1) {
                logexit("send");
                 }
                goto start;







            }
            if(sensorexiste==0){
                
                    char mensagemsucesso[BUFSZ];
                    snprintf(mensagemsucesso,BUFSZ,"no sensors");

                    count = send(csock, mensagemsucesso, strlen(mensagemsucesso) + 1, 0);
                    //printf("sucessfull enviado para o cliente");
                
                    if (count != strlen(mensagemsucesso) + 1) {
                    logexit("send"); 
                    }
                    
                goto start;
                 //close(csock);
                        
                    }


        }


















            
            //-----------------------------------------------------------------------




            // Exemplo de resposta genérica:
           /* else{
                const char *response = "Received your message!";
            count1 = send(csock, response, strlen(response) + 1, 0);
            if (count1 != strlen(response) + 1) {
                logexit("send");
            }
            break;
            }
    
            */

            //-------------FIM ---------------------------------------------------------------------------------------------------------
        

        // Fechar o socket do cliente após a comunicação.
        //close(csock);
    }

    //exit(EXIT_SUCCESS);
}

}