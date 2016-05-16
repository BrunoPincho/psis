#include "psiskv.h"

int main(){
	int socket;
	int port;
	char valor[30];
	char *msg;
	
	//int modo;
	//char *valu = valor;
	
	//uint32_t key;
	printf("porta?\n");
	scanf("%d",&port);
	
	socket=kv_connect("127.0.1.1",port);
	
	
	printf("socket : %d\n",socket);
	
	//write(socket,"hello\n",7);
	
	bzero(valor,30);
	/*printf("mensagem e modoe chave?\n");
	scanf("%s %d %u",msg,&modo,&key);
	int u=strlen(msg);
	printf("vai enviar isto %s de tamanho %d",msg,u);*/
	//length=6;
	
	if((kv_write(socket,19,"ola\0",4,0))<0)
		exit(0);
	sleep(1);
	if((kv_write(socket,19,"ola2\0",5,0))<0)
		exit(0);
	sleep(1);
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
	sleep(1);
	
	//kv_read(socket,20,msg,4);	
	
	//kv_delete(socket,20);	
	/*	
	if((kv_write(socket,70,"ola3\0",strlen("ola3\0"),0))<0)
		exit(0);	
	
	if((kv_write(socket,60,"ola4\0",strlen("ola4\0"),0))<0)
		exit(0);	*/
		
	puts("passou aqui");
	int x;
	printf("sair?\n");
	scanf("%d",&x);
	if(x==1)
		kv_close(socket);
	
	
	//printf("lido: %s\n",valu);
	
	exit(0);
}
