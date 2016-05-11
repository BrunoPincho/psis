#include "threadserv.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
LIST* LISTA = NULL;
LIST** list=&LISTA;
pthread_t *thread;

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
		printf("child exited, rebooting \n");
		Reboot();
	}
}


void dead_parent(int sig_num){
	sig_num=0;
		Reboot();
	
}

void Frontserver(){
		int socket;
		quit=0;
		puts("entrou front_server");
		
		int fd;
		//fd = udp_cliente();
		char resposta[5]="\0";
		
		//fifo_IN = open(FIFO_NAME, O_WRONLY);
		//fifo_OUT = open(FIFO_NAME,O_RDONLY);
		
		//close(fd[0]);
		
		//esta função é um system call que faz com que a morte do processo pai, do processo que chama a função, seja transmitida pelo kernel ao processo filho atraves do sinal SIGHUP
		prctl(PR_SET_PDEATHSIG, SIGHUP);
		
		////sinal de termino de um processo filho
		signal(SIGCHLD,dead_child);
		////
		
		/////sinal de termino de um processo pai
		signal(SIGHUP,dead_parent);
		/////
		
		pthread_create(&teclas,NULL,ler_teclado,&fd);
		sum_trd++;
		//int n;
		cria_server(1);
		int addrlen1;
	
			while(1){
					
					addrlen1=sizeof(servsoc);
					
					if((socket=accept(fs,(struct sockaddr*)&servsoc,(socklen_t *)&addrlen1))==-1){
								puts("sai no ACCEPT");				
								exit(1);
						}
						
					puts("ligou-se\n");	
					acede_shmem(resposta);
					/*if((sendto(fd,"P",2,0,(struct sockaddr*)&front_addr,sizeof(front_addr))<0)){
							printf("erro send\t");
						}*/
					
					//n=recvfrom(fd,resposta,5,0,(struct sockaddr*)&front_addr,((socklen_t *)&addrlen1));
					
					/*n=read(pipd[0],resposta,5);
					if(n==-1){exit(0);}*/
					
					write(socket,resposta,5);		
					
			}
}


void DataServer(){
		pthread_t Master;
		int porta=0;
		puts("entrou data server");
		//struct sockaddr_in cli_addr;
		//int addrlen;
		quit=0;
		//int n;
		porta=cria_server(0);
		//socket_udp = udp_server();
		//char leitura[10];
		char responder[5];
		bzero(responder,5);
		pthread_create(&Master,NULL,Master_thread,NULL);		
		//addrlen=sizeof(cli_addr);
		
		prctl(PR_SET_PDEATHSIG, SIGHUP);
	
		////sinal de termino de um filho
		signal(SIGCHLD,dead_child);
		////
		
		/////sinal de termino de um processo pai
		signal(SIGHUP,dead_parent);
		/////
		//fifo_IN = open(FIFO_NAME, O_WRONLY);
		//fifo_OUT = open(FIFO_NAME,O_RDONLY);		
		if(cria_shmem(porta)<0){
				printf("erro na memoria\n");
				exit(0);
			}
		
		while(*shm != '*'){
					
			//n=recvfrom(socket_udp,leitura,10,0,(struct sockaddr*)&cli_addr,((socklen_t *)&addrlen));
			
			
			/*if(strncmp(leitura,"\0",1)==0){
				printf("ordem de saida\n");
				exit(0);
			}
			if(strcmp(leitura,"P")==0){
				sprintf(responder,"%d",porta);
				if(write(pipd[1],responder,strlen(responder)+1)<0)
					printf("erro send\t");
				//sendto(socket_udp,responder,strlen(responder)+1,0,(struct sockaddr*)&cli_addr,sizeof(cli_addr))
			}
			bzero(leitura,10);*/
									
		}
		*shm = ' ';
		free(thread);
		puts("Data server a sair\n");	
		shmctl(shmid,IPC_RMID,NULL);	
		close(ds);	
		exit(0);
}


int cria_server(int servidor){
	if(servidor==1){
	//meter noutro ficheiro 
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
			////
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
			

		/*if(bind (ds,(struct sockaddr *) &servsoc, sizeof(servsoc))<0){
			printf("erro bind DS \n");	
			exit(-1);
		}*/
		
		if(listen(ds,5)==-1)exit(1);
		
		return porta;
	}
	return -1;
}

void* thread_accept(void *sd){
	struct Pacote pacote;	
	//char* valor;
	char *buffer;
	int socket;
	socket=*((int *) sd);
	//valores do timeout de recepção
	struct timeval tv;
	fd_set tcpsock;	
	FD_ZERO(&tcpsock);
	tv.tv_sec=5;
	tv.tv_usec=0;
	FD_SET(socket,&tcpsock);
	int nsele;
	
	
	while(1){
		FD_ZERO(&tcpsock);
		FD_SET(socket,&tcpsock);
		nsele=select(socket+1,&tcpsock,0,0,&tv);
		if(nsele<0){
			printf("erro no selec de timeout\n");
			puts("thread a sair");
					close(socket);
					sum_trd--;
					pthread_exit(NULL);
					return NULL;
				}
		if(nsele==0){
				puts("cliente timedout ,disconnecting\n");
				close(socket);
				sum_trd--;
				pthread_exit(NULL);
				return NULL;
				}
			
			
						
				
				read(socket,&pacote,sizeof(pacote));
				printf("lido %d, tamanho %d e modo %c\n",pacote.key,pacote.value_length,pacote.modo);
				//printf("value :%s\n",buffer);
				
				if(pacote.modo =='W'){
									buffer=(char*)malloc(pacote.value_length*sizeof(char));
									read(socket,buffer,pacote.value_length);
									printf("value: %s\n",buffer);
									
											
									list = novalor(list,pacote.key,buffer,pacote.value_length);
									imprimeList(*list);
									free(buffer);
									write(socket,"ack",3);
							}
				if(pacote.modo =='O'){
									buffer=(char*)malloc(pacote.value_length*sizeof(char));
									read(socket,buffer,pacote.value_length);
									printf("value: %s\n",buffer);
									
									list = altera(list,pacote.key,buffer,pacote.value_length);
									imprimeList(*list);
									free(buffer);
									write(socket,"ack",3);
								}
								
				if(pacote.modo == 'R'){
									buffer=(char*)malloc(pacote.value_length*sizeof(char));
									strcpy(buffer,procura(*list,pacote.key));
									printf("%s\n",buffer);
									
									if(write(socket,buffer,pacote.value_length)<0)
										exit(0);
										
									free(buffer);	
						}				
				if(pacote.modo == 'D'){				
									printf("vai ser apagado o valor cuja key é %d",pacote.key);
									/****inserir o codigo que vai à lista e apaga os valores***/
									write(socket,"ack",3);					
					}
				
								
				
				/*pthread_mutex_lock(&mux);
					printf("%d\n",sincro.valor);
					sincro.valor++;
				pthread_mutex_unlock(&mux);	*/	
				
		//printf("lido %d, tamanho %d e modo %c\n",pacote.key,pacote.value_length,pacote.modo);
				/*pacote.value=(char*)malloc(pacote.value_length*sizeof(char));
				if(read(socket,pacote.value,pacote.value_length+1)<0)
							exit(0);
				printf("recebido %s \n",pacote.value);
				free(pacote.value);
				memset(&pacote,0,sizeof(pacote));*/
					/*
					 * n=read(socket,&pacote,sizeof(pacote));
					if(pacote.value_length!=0){				
						if(pacote.modo =='W' || pacote.modo =='O'){	
								pacote.value=(char*)malloc(pacote.value_length*sizeof(char));
											
											if(write(socket,"Ack\0",4)<0)
													exit(0);
												
											if(read(socket,pacote.value,pacote.value_length+1)<0)
													exit(0);
						
							
						
							
											
						printf("lido %s, tamanho %d e modo %c\n",pacote.value,pacote.value_length,pacote.modo);
						
						if(pacote.modo =='W'){
							
									
											
									list = novalor(list,pacote.key,pacote.value,pacote.value_length);
									imprimeList(*list);
									free(pacote.value);
							}
							
						if(pacote.modo =='O'){
								
									
									list = altera(list,pacote.key,pacote.value,pacote.value_length);
									imprimeList(*list);
									free(pacote.value);
								}
							}	
						if(pacote.modo == 'R'){
									pacote.value=(char*)malloc(pacote.value_length*sizeof(char));
									strcpy(pacote.value,procura(*list,pacote.key));
									printf("%s\n",pacote.value);
									
									if(write(socket,pacote.value,pacote.value_length+1)<0)
										exit(0);
										
									free(pacote.value);	
						}
						
						
						
						//memset(&pacote,0,sizeof(pacote));
					}*/
				
					
			}

		return NULL;
}

void *Master_thread(){
	int thread_size;
	thread_size = nthread;
	int sock,addrlen;
	thread=(pthread_t *)malloc(sizeof(pthread_t)*nthread);
		
	while(1){
			addrlen=sizeof(servsoc);
			if((sock=accept(ds,(struct sockaddr*)&servsoc,(socklen_t *)&addrlen))==-1){
								puts("sai no ACCEPT");				
								exit(1);
				}
					
					//sscanf(leitura,"%d",&sock);		
					//
					
				printf("connectada a socket:%d\n",sock);			
				printf("criada thread #%d\n",sum_trd);
				pthread_create(&thread[sum_trd],NULL,thread_accept,&sock);
				sum_trd++;	
			if(sum_trd>thread_size){
				thread_size+=nthread;
				thread=realloc(thread,sizeof(pthread_t)*thread_size);
			}
		}
	
	/*for(i=0;i<sum_trd;i++){
			pthread_join(thread[i],NULL);
		}*/
}



void *ler_teclado(void *fd){
	char mensagem[128];
	//int dataserver;
	//dataserver=*((int *) fd);

	puts("a ler do teclado\n");
	while(fgets(mensagem,128,stdin)!=NULL){

		printf("\nfoi escrito: %s \n",mensagem);
		if(strncmp(mensagem,"quit",4)==0){
					quit=1;
			//if((sendto(dataserver,"\0",2,0,(struct sockaddr*)&front_addr,sizeof(front_addr))<0))
					terminu_shmem();	
						
					printf("Front a sair\n");
			
			close(fs);
			exit(0);
			
			}
	}	
	return NULL;
}

/*
int udp_server(){
	
	int sockfd;
	struct sockaddr_in serv_addr;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0))<0)
		perror("socket\n");
	
	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr= htonl(INADDR_ANY);
	serv_addr.sin_port=htons(8002);
	
	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
		printf("error bind udp\n");
	}
	
	return sockfd;
}
*/

/*
int udp_cliente(){
	int sockfd;
	
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0))<0)
		perror("socket\n");
	
	bzero((char*)&front_addr,sizeof(front_addr));
	front_addr.sin_family = AF_INET;
	front_addr.sin_addr.s_addr= inet_addr("127.0.1.1");
	front_addr.sin_port=htons(8002);
	
	
	return sockfd;
}*/

////MEMORIA PARTILHADA

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
}