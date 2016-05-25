#include "psiskv.h"


int kv_connect(char* kv_server_ip,int kv_server_port){
	int sd,data_server;
	char porta[5];
	int port;
	struct sockaddr_in serv_soc,data_soc;
	/**************cria socket******************/
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0)
		return -1;
	
	if((data_server=socket(AF_INET,SOCK_STREAM,0))<0)
		return -1;	
		
	/********************************************/	
	
	/************estruturar o sockadr************/
	
	bzero((char*)&serv_soc,sizeof(serv_soc));
	
	serv_soc.sin_family=AF_INET;
	serv_soc.sin_addr.s_addr=inet_addr(kv_server_ip);
	serv_soc.sin_port=htons(kv_server_port);
	/********************************************/
	
	if(connect(sd,(struct sockaddr*)&serv_soc,sizeof(struct sockaddr))<0){
		return -1;
		}
		
	read(sd,porta,5);
	sscanf(porta,"%d",&port);
	
	close(sd);
	
	bzero((char*)&data_soc,sizeof(data_soc));
	
	data_soc.sin_family=AF_INET;
	data_soc.sin_addr.s_addr=inet_addr(kv_server_ip);
	data_soc.sin_port=htons(port);
	
	if(connect(data_server,(struct sockaddr*)&data_soc,sizeof(struct sockaddr))<0){
			return -1;
		}
	
	return data_server;
}



void kv_close(int kv_descriptor){
	memset(&pacote,0,sizeof(pacote));
	pacote.modo='\0';
	if((write(kv_descriptor,&pacote,sizeof(pacote)))<0){
		puts("erro a desconnectar\n");
		return;
	}
	close(kv_descriptor);
	return;
}

int kv_write(int kv_descriptor,uint32_t key,char* value,uint32_t value_length,int kv_overwrite){
	char ACK[4];
	struct timeval tv;
	int nsele;
	bzero(ACK,4);
	
	fd_set tcpsock;	
	FD_ZERO(&tcpsock);
	tv.tv_sec=3;
	tv.tv_usec=0;
	FD_SET(kv_descriptor,&tcpsock);
	memset(&pacote,0,sizeof(pacote));

	if(kv_overwrite==1){
			pacote.modo='O';
		}else{
			pacote.modo='W';
		}
		
		pacote.key=key;
		pacote.value_length=value_length;

	strcat(value,"\0");

	if((write(kv_descriptor,&pacote,sizeof(pacote)))<0){
		puts("erro a enviar pacote\n");
		return -1;
	}
	
	if((write(kv_descriptor,value,value_length))<0){
		puts("erro a enviar value\n");
		return -1;
	}


	nsele=select(kv_descriptor+1,&tcpsock,0,0,&tv);
	
	if(nsele==0){
		puts("key value storage offline,socket fechada\n");
		close(kv_descriptor);
		return -1;
	}
	
	if(nsele<0){
		puts("erro no selec de timeout\n");
		return -1;
	}




	if(read(kv_descriptor,ACK,4)<0){
		puts("lixou read");
		}

	if(strncmp(ACK,"n",1)==0){
		puts("Ja existe esta chave\n");
		return -2;
	}
	
	if(strncmp(ACK,"ack",3)==0){
		puts("sucesso no write\n");		
		return 0;
	}
	
	
	
	return -1;
}

int kv_read(int kv_descriptor, uint32_t key,char* value, uint32_t value_length){
	struct timeval tv;
	int nsele;
	fd_set tcpsock;	
	FD_ZERO(&tcpsock);
	tv.tv_sec=3;
	tv.tv_usec=0;
	FD_SET(kv_descriptor,&tcpsock);
		
	pacote.modo='R';	
	pacote.key=key;
	pacote.value_length=value_length;
	
	
	if((write(kv_descriptor,&pacote,sizeof(pacote))<0)){
		puts("erro a enviar\n");
		return -1;
	}
	
	nsele=select(kv_descriptor+1,&tcpsock,0,0,&tv);
	
	if(nsele==0){
		puts("key value storage offline,socket fechada\n");
		close(kv_descriptor);
		
		return -1;
	}
	
	if(nsele<0){
		puts("erro no selec de timeout\n");
	
		return -1;
	}
	
	if((read(kv_descriptor,value,value_length))<0){
		puts("falhou o read");
		
		return -1;	
	}
	
	if(strcmp(value,"\0")==0){
		puts("valor não existente");
		return -2;
	}
	
	
	

	return strlen(value);
}


int kv_delete(int kv_descriptor,uint32_t key){
	memset(&pacote,0,sizeof(pacote));
	pacote.modo='D';
	pacote.key=key;
	char Ack[3];

	struct timeval tv;
	int nsele;
	fd_set tcpsock;	
	FD_ZERO(&tcpsock);
	tv.tv_sec=3;
	tv.tv_usec=0;
	FD_SET(kv_descriptor,&tcpsock);
	
	if(write(kv_descriptor,&pacote,sizeof(pacote))<0){
		puts("erro a enviar\n");
		return -1;
	}

	nsele=select(kv_descriptor+1,&tcpsock,0,0,&tv);

	if(nsele==0){
		puts("key value storage offline,socket fechada\n");
		close(kv_descriptor);
		
		return -1;
	}
	
	if(nsele<0){
		puts("erro no selec de timeout\n");
	
		return -1;
	}
	
	if(read(kv_descriptor,Ack,3)<0){
		puts("erro a ler\n");
		return -1;
	}

	if(strncmp(Ack,"ack",3)==0){
		puts("valor apagado da key value storage\n");
		return 0;
	}
	
	return -1;
}
