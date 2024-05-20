#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void usage(int argc, char **argv) {
    printf("usage: %s <server IP> <server port>\n", argv[0]);
    printf("example: %s 127.0.0.1 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

#define BUFSZ 500

int main(int argc, char **argv) {
    if (argc < 3) {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != addrparse(argv[1], argv[2], &storage)) {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }
    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != connect(s, addr, sizeof(storage))) {
        logexit("connect");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);

    printf("connected to %s\n", addrstr);

	

    while (1) {
		
        char buf[BUFSZ];
        memset(buf, 0, BUFSZ);
		
        printf("mensagem> ");
		int modo1=0,modo2=0,modo3=0,modo4=0,modo5=0,modo6=0,modo7=0,modo8=0;
		int killrecebido=0;
		int invalidsensor=0;
        fgets(buf, BUFSZ - 1, stdin);//aqui pega a mensagem do teclado no cliente
		int valor1=-1,valor2=-1,valor3=-1,valor4=-1;
		size_t count;
		char arquivo[BUFSZ];
		char arquivomudanca[BUFSZ];
		//------------------------PROCESSAMENTO DAS MENSAGENS ENVIADAS PELO CLIENTE DE ACORDO COM OS REQUISITOS ----------------
		

		if ((sscanf(buf, "install param %d %d %d %d", &valor1, &valor2, &valor3, &valor4) ==4)||
		(sscanf(buf, "install param %d %d %d", &valor1, &valor2, &valor3)==3)||
		(sscanf(buf, "install param %d %d", &valor1, &valor2)==2)||
		(sscanf(buf, "install param %d", &valor1) ==1)||
		strcmp(buf,"install param\n")==0) {
		modo1=1;
		if(valor1<0){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
		}

		else if(valor2<0 || valor2>10){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
		}
		else if(valor3<0 || valor3>150){
			invalidsensor=1;
			printf("invalid sensor\n"); 
		
		}
		else if(valor4<0 || valor4>100){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
		}

		else{
			char enviaINS_REQ[100];
			sprintf(enviaINS_REQ, "INS_REQ %d %d %d %d", valor1, valor2, valor3, valor4);

        	//printf("Comando formatado enviado para o servidor: %s\n", enviaINS_REQ);

			count = send(s, enviaINS_REQ, strlen(enviaINS_REQ)+1, 0);
			if (count != strlen(enviaINS_REQ)+1) {
		    logexit("send");
			}
			
		}

		}


		










		
		
		if ((sscanf(buf, "install file %s", arquivo) == 1)||(strcmp(buf,"install file\n")==0)) {
			modo2=1;
			FILE *arquivoPtr= fopen(arquivo,"r");
			/*if(arquivoPtr==NULL){
				printf("Arquivo solicitado não existe , por favor tente novamente !");
			}*/
			if(arquivoPtr != NULL){
				char conteudo[BUFSZ];
				size_t tamanho;

				tamanho=fread(conteudo,1,sizeof(conteudo),arquivoPtr);
				
			if ((sscanf(conteudo, "%d %d %d %d", &valor1, &valor2, &valor3, &valor4) == 4)||(sscanf(conteudo,"%d %d %d",&valor1,&valor2,&valor3)==3)||(sscanf(conteudo,"%d %d",&valor1,&valor2)==2)||(sscanf(conteudo,"%d",&valor1)==1)){
			fclose(arquivoPtr);

			if(valor1<0){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
			}
			
			else if(valor2<0 || valor2>10){
				invalidsensor=1;
				printf("invalid sensor\n"); 
				
			}
			else if(valor3<0 || valor3>150){
				invalidsensor=1;
				printf("invalid sensor\n"); 
				
			}
			else if(valor4<0 || valor4>100){
				invalidsensor=1;
				printf("invalid sensor\n"); 
				
			}
			else{
				char enviaINS_REQ[100];
				sprintf(enviaINS_REQ, "INS_REQ %d %d %d %d", valor1, valor2, valor3, valor4);

				//printf("Comando formatado enviado para o servidor: %s\n", enviaINS_REQ);

				count = send(s, enviaINS_REQ, strlen(enviaINS_REQ)+1, 0);
				if (count != strlen(enviaINS_REQ)+1) {
				logexit("send");
				}
				
			}


			}

				
			}
			else{
				invalidsensor=1;
				printf("invalid sensor\n");
			}
			
	}
	



		if (sscanf(buf, "change param %d %d %d %d", &valor1, &valor2, &valor3, &valor4) == 4||
		(sscanf(buf, "change param %d %d %d", &valor1, &valor2, &valor3)==3)||
		(sscanf(buf, "change param %d %d", &valor1, &valor2)==2)||
		(sscanf(buf, "change param %d", &valor1) ==1)||
		strcmp(buf,"change param\n")==0) {
			modo3=1;
		
		if(valor1<0){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
		}
		else if(valor2<0 || valor2>10){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
		}
		else if(valor3<0 || valor3>150){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
		}
		else if(valor4<0 || valor4>100){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
		}
		else{
			char enviaINS_REQ[100];
			sprintf(enviaINS_REQ, "CH_REQ %d %d %d %d", valor1, valor2, valor3, valor4);

        	//printf("Comando formatado enviado para o servidor: %s\n", enviaINS_REQ);

			count = send(s, enviaINS_REQ, strlen(enviaINS_REQ)+1, 0);
			if (count != strlen(enviaINS_REQ)+1) {
		    logexit("send");
			}
		}
		//break;

	}

		if(strcmp(buf,"change file\n")==0){
			invalidsensor=1;
			printf("invalid sensor\n");
			
		}

		
		if ((sscanf(buf, "change file %s", arquivomudanca) == 1)) {
			modo4=1;
			FILE *arquivoPtr= fopen(arquivomudanca,"r");
			if(arquivoPtr != NULL){
				char conteudo[BUFSZ];
				size_t tamanho;

				tamanho=fread(conteudo,1,sizeof(conteudo),arquivoPtr);
				
			if ((sscanf(conteudo, "%d %d %d %d", &valor1, &valor2, &valor3, &valor4) == 4)||(sscanf(conteudo,"%d %d %d",&valor1,&valor2,&valor3)==3)||(sscanf(conteudo,"%d %d",&valor1,&valor2)==2)||(sscanf(conteudo,"%d",&valor1)==1)){
			//fclose(arquivoPtr);

			if(valor1<0){
			invalidsensor=1;
			printf("invalid sensor\n"); 
			
			}
			
			
			else if(valor2<0 || valor2>10){
				invalidsensor=1;
				printf("invalid sensor\n"); 
				
			}
			else if(valor3<0 || valor3>150){
				invalidsensor=1;
				printf("invalid sensor\n"); 
				
			}
			else if(valor4<0 || valor4>100){
				invalidsensor=1;
				printf("invalid sensor\n"); 
				
			}
			else{
				char enviaINS_REQ[100];
				sprintf(enviaINS_REQ, "CH_REQ %d %d %d %d", valor1, valor2, valor3, valor4);

				//printf("Comando formatado enviado para o servidor: %s\n", enviaINS_REQ);

				count = send(s, enviaINS_REQ, strlen(enviaINS_REQ)+1, 0);
				if (count != strlen(enviaINS_REQ)+1) {
				logexit("send");
				}
				
			}


			}

				
			}
			else{
				invalidsensor=1;
				printf("invalid sensor\n");
			}

			fclose(arquivoPtr);
			arquivoPtr == NULL;
	}


	 if ((sscanf(buf,"show value %d", &valor1)==1)) {
		modo5=1;
			
			char enviaINS_REQ[100];
			sprintf(enviaINS_REQ, "SEN_REQ %d", valor1);

        	//printf("Comando formatado enviado para o servidor: %s\n", enviaINS_REQ);

			count = send(s, enviaINS_REQ, strlen(enviaINS_REQ)+1, 0);
			if (count != strlen(enviaINS_REQ)+1) {
		    logexit("send");
			}
			
			
			/*else{
				invalidsensor=1;
				printf("invalid sensor\n");
			}*/
			
			//break;
		}



	 if(strcmp(buf,"show values\n") == 0) {
		modo6=1;
		
		
			char enviaINS_REQ[100];
			sprintf(enviaINS_REQ, "VAL_REQ");

        	//printf("Comando formatado enviado para o servidor: %s\n", enviaINS_REQ);

			count = send(s, enviaINS_REQ, strlen(enviaINS_REQ)+1, 0);
			if (count != strlen(enviaINS_REQ)+1) {
		    logexit("send");
			}
			
			//break;
		}
	


	 if((sscanf(buf, "remove %d", &valor1) == 1)||(strcmp(buf,"remove\n")==0)) {
		modo7=1;
		
			if(valor1>0){ 
			char enviaINS_REQ[100];
			sprintf(enviaINS_REQ, "REM_REQ %d", valor1);

        	//printf("Comando formatado enviado para o servidor: %s\n", enviaINS_REQ);

			count = send(s, enviaINS_REQ, strlen(enviaINS_REQ)+1, 0);
			if (count != strlen(enviaINS_REQ)+1) {
		    logexit("send");
			}
			}
			else{
				invalidsensor=1;
				printf("invalid sensor\n");
			}
			
			//break;
		}


		if (strcmp(buf, "kill\n") == 0) {
    char enviaINS_REQ[] = "kill"; // Sem espaço após "kill"

    // Use sizeof(enviaINS_REQ) - 1 para excluir o caractere nulo '\0'
    count = send(s, enviaINS_REQ, sizeof(enviaINS_REQ) - 1, 0);
    if (count != sizeof(enviaINS_REQ) - 1) {
        logexit("send");
    }
}
		
		


	












		//-----------------------FIM PROCESSAMENTO---------------------------------------------------------------------------------
        

		if((modo1==0)&&(modo2==0)&&(modo3==0)&&(modo4==0)&&(modo5==0)&&(modo6==0)&&(modo7==0)) {
			//printf("depurando");
			close(s);
			exit(EXIT_FAILURE);
			break;
		
		}

        memset(buf, 0, BUFSZ);
        unsigned total = 0;

		//----------------------FIM DO ENVIO DA MENSAGEM SEM NENHUM PROTOCOLO------------------------------------------------------
		int saidowhile=1;
		
		
        while (1) {
			if(invalidsensor==1){
				break;
			}
			
			
            count = recv(s, buf + total, BUFSZ - total, 0);


			if (strcmp(buf, "kill") == 0){
				killrecebido=1;
				break;

			 }
			 //printf("ESTOU ESPERANDO MENSAGEM DO SERVIDOR\n");
			 
			

            if (count == 0) {
                // Connection terminated.
                break;
            }

            total += count;
			

            // Verifique se a mensagem recebida é completa.
            if (buf[total - 1] == '\0') {
                break;
            }

			
			
        }
		

		
		
        //printf("received %u bytes\n", total);
		/*if(total==0){
			break;
		}*/
		if(killrecebido==1){
			break;
		}
		if((invalidsensor==0)&&(killrecebido==0)){
			int aux1,aux2,aux3;
			 if (sscanf(buf,"SEN_RES %d %d %d", &aux1,&aux2,&aux3) == 3){
				printf("sensor %d: %d %d\n",aux1,aux3,aux2);
			 }

			
			else if (strncmp(buf, "VAL_RES", 7) == 0) {
    // A mensagem começa com "VAL_RES"
    char *ptr = buf + 8;  // Avança para o caractere após "VAL_RES"

    // Pule quaisquer espaços em branco no início da mensagem
    /*while (*ptr == ' ') {
        ptr++;
    }*/

    // Use sscanf para ler os valores até o final da mensagem
    int value;
	int vetaux[BUFSZ]={0};
	int i=0;
     while (sscanf(ptr, " %d", &value) == 1) {
        //printf("Sensors: %d\n", value);

        // Armazene o valor na posição atual em 'vetaux'
        vetaux[i] = value;
        

        // Avance para o próximo valor (se houver)
        ptr = strchr(ptr, ' ');
        if (ptr == NULL) {
            break;  // Não há mais espaços, saia do loop
        }

        // Avance para o próximo caractere após o espaço
        ptr++;
		i++;
		
    }
	
	
	printf("sensors: ");
	for(int j=0;j<i;j+=3){
		printf("%d (%d %d) ",vetaux[j],vetaux[j+1],vetaux[j+2]);

	}
	
	printf("\n");





			}

			else{ 
			puts(buf);
			}
		}
        
		
		
    }

    //exit(EXIT_SUCCESS);
}
