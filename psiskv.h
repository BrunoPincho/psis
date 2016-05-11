#ifndef PSISKV_H
#define PSISKV_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>


int kv_connect(char* kv_server_ip,int kv_server_port);
void kv_close(int kv_descriptor);
int kv_write(int kv_descriptor,uint32_t key,char* value,uint32_t value_length,int kv_overwrite);
int kv_read(int kv_descriptor, uint32_t key,char* value, uint32_t value_length);
int kv_delete(int kv_descriptor,uint32_t key);
int* serializacao(char *msg);

struct Pacote{
	uint32_t key;
	uint32_t value_length;
	//char* value;
	char modo;
};

struct Pacote pacote;

#endif
