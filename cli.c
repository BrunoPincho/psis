#include "psiskv.h"
#define MAX_VALUES 10
#include <limits.h>

int main(){
	int socket;
	int port;
	char linha[130];
	int x;
	//char *msg;
	uint32_t key;
	//int modo;
	//char *valu = valor;
	
	//uint32_t key;
	printf("porta?\n");
	scanf("%d",&port);
	
	socket=kv_connect("127.0.1.1",port);
	
	
	printf("socket : %d\n",socket);
	/*
	for (uint32_t i = 0; i < 10; i ++){
		sprintf(linha, "%u", i);
		kv_write(socket, i , linha, strlen(linha)+1, 0);
		sleep(1);
	}
	*/
	
	
	
	/*printf("mensagem e modoe chave?\n");
	scanf("%s %d %u",msg,&modo,&key);
	int u=strlen(msg);
	printf("vai enviar isto %s de tamanho %d",msg,u);
	//length=6;
	key=190;
	if((kv_write(socket,key,"ola\0",4,0))<0)
		exit(0);
	sleep(1);
	if((kv_write(socket,key,"ola2\0",6,0))<0)
		exit(0);
	sleep(1);*//*
	if((kv_write(socket,21,"ola3\0",5,0))<0)
		exit(0);
	sleep(1);
	if((kv_write(socket,20,"ola4\0",5,0))<0)
		exit(0);
	sleep(1);
	if((kv_write(socket,21,"ola5\0",5,1))<0)
		exit(0);
	sleep(1);	
	if((kv_write(socket,190,"ola6\0",5,0))<0)
		exit(0);
	sleep(1);	
	if((kv_read(socket,21,msg,5))<0)
		exit(0);	
	sleep(1);*/
	
	//kv_read(socket,20,msg,4);	
	if((kv_write(socket,190,"ola9\0",6,0))<0){
		
	}
		sleep(1);
	if((kv_read(socket,190,linha,5))<0)
		exit(0);	
	printf("lido: %s\n",linha);
		sleep(1);	
	//kv_delete(socket,4);
	/*	
	if((kv_write(socket,70,"ola3\0",strlen("ola3\0"),0))<0)
		exit(0);	
	
	if((kv_write(socket,60,"ola4\0",strlen("ola4\0"),0))<0)
		exit(0);	*/
		
	puts("passou aqui");
	
	printf("sair?\n");
	scanf("%d",&x);
	if(x==1)
		kv_close(socket);
	
	
	//printf("lido: %s\n",valu);
	
	exit(0);
}
