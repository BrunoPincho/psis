#include "threadserv.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
LIST* LISTA = NULL;
LIST** list=&LISTA;
pthread_t *thread;
 sigset_t signal_mask,saved_mask;
int shmid;
char *shm;
pthread_t teclas;


void Reboot(){
	pid_t reboot;
	
	if((reboot = fork())==-1){
		perror("fork");
		exit(-1);
	}
	
	if(reboot==0){
		sleep(1);
			if(ds!=0){
				close(ds);			
				ds=0;
				Frontserver();
			}
			if(fs!=0){
				
				
				close(fs);
				fs=0;
				DataServer();			
			}		
	}else{
		
		return;
	}
}
	
void dead_child(int sig_num){
	
	int status;
	
	wait(&status);
	sig_num=0;
	if(*shm!='*'){
		if(quit!=1){
		printf("child exited, rebooting \n");
		Reboot();
		}
	}
}


void dead_parent(int sig_num){
	sig_num=0;
	if(quit!=1){
		Reboot();
	}
	
}



void Frontserver(){
		int socket;
		quit=0;
		puts("entrou front_server");
		
		int fd;
		char resposta[5]="\0";

		
		/*esta função é um system call que faz com que a morte do processo pai, do processo que chama a função, seja transmitida pelo kernel ao processo filho atraves do sinal SIGHUP*/
		prctl(PR_SET_PDEATHSIG, SIGHUP);
		
		/*sinal de termino de um processo filho*/
		signal(SIGCHLD,dead_child);
		
		
		/*sinal de termino de um processo pai*/
		signal(SIGHUP,dead_parent);
		
		
		pthread_create(&teclas,NULL,ler_teclado,&fd);
		sum_trd++;
		
		cria_server(1);
		int addrlen1;
	
			while(quit!=1){
					
					addrlen1=sizeof(servsoc);
					
					if((socket=accept(fs,(struct sockaddr*)&servsoc,(socklen_t *)&addrlen1))==-1){
								puts("sai no ACCEPT");				
								break;
						}
						
					puts("ligou-se\n");	
					acede_shmem(resposta);
					
					
					write(socket,resposta,5);		
					
			}
			pthread_join(teclas,NULL);
			exit(0);
}


void DataServer(){
		pthread_t Master;
		int porta=0;
		puts("entrou data server");
		
		quit=0;

		porta=cria_server(0);

		

		char responder[5];
		bzero(responder,5);
		pthread_create(&Master,NULL,Master_thread,NULL);		
		/***********************************/
		
		prctl(PR_SET_PDEATHSIG, SIGHUP);
	
		/*sinal de termino de um filho*/
		signal(SIGCHLD,dead_child);
		
		
		/*sinal de termino de um processo pai*/
		signal(SIGHUP,dead_parent);
	    
		
			
		if(cria_shmem(porta)<0){
				printf("erro na memoria\n");
				exit(0);
			}
			
		ler_logfile();
		
		while(*shm != '*'){
					
			
									
		}
		quit=1;
		*shm = ' ';
		free(thread);
		puts("Data server a sair\n");	
		matalista(list);
		shmctl(shmid,IPC_RMID,NULL);	

		fclose(logd);
		close(ds);	
		exit(0);
}


int cria_server(int servidor){
	if(servidor==1){

			if((fs = socket(AF_INET,SOCK_STREAM,0))<0){
			puts("erro no socket");
			exit(-1);
			}		

			printf("socket tcp nº %d criada\n",fs);

			memset((void*)&servsoc,(int)'\0',sizeof(servsoc));
			servsoc.sin_family = AF_INET;
			servsoc.sin_addr.s_addr = htonl(INADDR_ANY);
			int port=9999;
			servsoc.sin_port = htons(port);

			while(bind (fs,(struct sockaddr *) &servsoc, sizeof(servsoc))==-1){
				printf("erro bind\t mudar a porta\n");	
				port--;
				servsoc.sin_port = htons(port);
			}
			printf("porta %d \n",port);

			if(listen(fs,5)==-1)exit(1);
			return port;
	}
	
	if(servidor==0){
		int porta=9000;
		
			if((ds = socket(AF_INET,SOCK_STREAM,0))<0){
		puts("erro no socket");
		exit(-1);
		}		

		printf("socket tcp nº %d criada\n",ds);

		memset((void*)&servsoc,(int)'\0',sizeof(servsoc));
		servsoc.sin_family = AF_INET;
		servsoc.sin_addr.s_addr = htonl(INADDR_ANY);
		servsoc.sin_port = htons(porta);
		
		while(bind (ds,(struct sockaddr *) &servsoc, sizeof(servsoc))==-1){
				printf("erro bind do Data_s\t mudar a porta\n");	
				porta++;
				servsoc.sin_port = htons(porta);
			}
		
		if(listen(ds,5)==-1)exit(1);
		
		return porta;
	}
	return -1;
}



void* thread_accept(void *sd){
	struct Pacote pacote;	
	char *buffer;
	int socket;
	socket=*((int *) sd);
	int lido;
		
		
		
	
	while(1){
		
		
				
				lido=recv(socket,&pacote,sizeof(pacote),MSG_NOSIGNAL);
				if(lido<=0){
					printf("Ocorreu erro no receive\n");
					close(socket);
					printf("thread nº %d a sair\n",sum_trd);
					sum_trd--;
					pthread_exit(NULL);
				}

				printf("lido %d, tamanho %d e modo %c\n",pacote.key,pacote.value_length,pacote.modo);

					switch(pacote.modo){
						case 'W':									
										buffer=(char*)malloc(pacote.value_length*sizeof(char));
										bzero(buffer,pacote.value_length);
										read(socket,buffer,pacote.value_length);
										printf("value: %s\n",buffer);
										if(strcmp(buffer,procura(*list,pacote.key))==0){
											write(socket,"n",1);
											free(buffer);	
										}else{
											list = novalor(list,pacote.key,buffer,pacote.value_length);
											//imprimeList(*list);
											update_log(pacote.modo,pacote.key,buffer,pacote.value_length);
											free(buffer);
											write(socket,"ack",3);
										}
										break;
									
						case 'O':
									buffer=(char*)malloc(pacote.value_length*sizeof(char));
									read(socket,buffer,pacote.value_length);
									printf("value: %s\n",buffer);

									if(strcmp(procura(*list,pacote.key),"\0")==0){
										write(socket,"n",1);
										free(buffer);
										break;
									}else{
										list = altera(list,pacote.key,buffer,pacote.value_length);
										//imprimeList(*list);
										update_log(pacote.modo,pacote.key,buffer,pacote.value_length+1);
										free(buffer);
										write(socket,"ack",3);
									}
									break;
						case 'R':		
									buffer=(char*)malloc(pacote.value_length*sizeof(char));
									strcpy(buffer,procura(*list,pacote.key));
									printf("%s\n",buffer);
									
									if(write(socket,buffer,pacote.value_length)<0)
										exit(0);
									
									free(buffer);	
									break;

						case 'D':		
									if(strcmp(procura(*list,pacote.key),"\0")==0){
										write(socket,"n",1);
										break;
									}else{
										printf("vai ser apagado o valor cuja key é %d",pacote.key);
										eliminar(list,pacote.key);
										update_log(pacote.modo,pacote.key,"\0",0);
										//imprimeList(*list);
										write(socket,"ack",3);
									}		
									break;
			
						case '\0':
									printf("thread nº %d a sair\n",sum_trd);
									close(socket);
									sum_trd--;
									pthread_exit(NULL);
									return NULL;		
									break;	
				}		
			}

		return NULL;
}

void *Master_thread(){
	int thread_size;
	thread_size = nthread;
	int sock,addrlen;

	thread=(pthread_t *)malloc(sizeof(pthread_t)*nthread);

	/*struct timeval timeout;
	timeout.tv_sec=10;
	timeout.tv_usec=0;*/




		
	while(1){
			addrlen=sizeof(servsoc);
			if((sock=accept(ds,(struct sockaddr*)&servsoc,(socklen_t *)&addrlen))==-1){
								puts("sai no ACCEPT");				
								exit(1);
				}
				//setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));	
				printf("connectada a socket:%d\n",sock);			
				printf("criada thread #%d\n",sum_trd);
				pthread_create(&thread[sum_trd],NULL,thread_accept,&sock);
				sum_trd++;	
			if(sum_trd==thread_size){
				thread_size+=nthread;
				printf("*** numero maximo de threads atingido, alocando mais %d threads***\n",thread_size);
				thread=realloc(thread,sizeof(pthread_t)*thread_size);
			}
			
		}
			puts("libertar threads\n");
				free(thread);
				pthread_exit(NULL);

}

void *ler_teclado(void *fd){
	char mensagem[128];

	puts("a ler do teclado\n");
	while(fgets(mensagem,128,stdin)!=NULL){

		printf("\nfoi escrito: %s \n",mensagem);
		if(strncmp(mensagem,"quit",4)==0){
					quit=1;
					terminu_shmem();	
						
					printf("Front a sair\n");
			
			close(fs);
			sleep(0.1);
			pthread_exit(NULL);
			
			}
	}	
	return NULL;
}




/***************************************MEMORIA PARTILHADA****************************************************/

int cria_shmem(int porta){
	
    key_t key;
    char  *s;
	int i;
	
	char port[4];
	sprintf(port,"%d",porta);
	
	key = 69;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, 10, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return -1;
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return -1;
    }
	
	s=shm;
	
    for (i=0; i < 4; i++)
        *s++ = port[i];
	
	*s = '\0';
	return 1;
	
}

void acede_shmem(char* porta){
	key_t key=69;
	int shmid;
    char *shm, *s;
    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, 10, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now read what the server put in the memory.
     */
     int i=0;
    for (s = shm; *s != '\0'; s++){
		porta[i]=*s;
        putchar(*s);
        i++; 
	}
return;
}



void terminu_shmem(){
	key_t key=69;
	int shmid;
    char *shm;
    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, 10, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
     *shm='*';
return;
}
/**********************************************LOG FILE****************************************************/

void ler_logfile(){
	
	logd = fopen("log.txt","a+");
	if(logd==NULL){
		puts("log não existente,a criar\n");
		cria_log();
		return;
	}
	
	char *temp;
	char dados[128];
	int comando;
	uint32_t key;
	uint32_t length;
	bzero(dados,128);
	puts("iniciar a leitura\n");
	
	
	while(fscanf(logd,"%d %d %d",&comando,&key,&length)>0){
		
			printf("dados: %d %d %d\n",comando,key,length);
		if(length!=0){
			temp=(char *)malloc(length*sizeof(char));
			fscanf(logd,"%s",temp);
			printf("value: %s\n",temp);
			}
		switch(comando){
			case 'W':	list=novalor(list,key,temp,length);
						free(temp);
						bzero(dados,128);
						comando=0;
						key=0;
						length=0;
						imprimeList(*list);
						break;
			case 'O':	list=altera(list,key,temp,length);
						free(temp);
						bzero(dados,128);
						comando=0;
						key=0;
						length=0;
						imprimeList(*list);
						break;
			case 'D':   
						eliminar(list,key);
						imprimeList(*list);
						
						break;
			}
		}
	
	imprimeList(*list);
	puts("******parou de ler o log*****\n");
return;
}

void cria_log(){
	if((logd=fopen("log.txt","w+"))==NULL){
		puts("falhou a abertura do log\n");
		
		return;
	}
}

void update_log(int comando,uint32_t key, char* valor,uint32_t length){	
	pthread_mutex_lock(&mux);
	 	fprintf(logd,"%d %d %d\n",comando,key,length);
	 	fprintf(logd,"%s\n",valor);
	pthread_mutex_unlock(&mux);
return;
}

