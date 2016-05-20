#include "threadserv.h"



/**************************************************************************************************************************/
//Cria um novo elemento na lista
LIST** novalor(LIST** a, uint32_t key,char* value,uint32_t value_len){

LIST* novop;
LIST* aux=NULL;
LIST* aux2=NULL;

aux=*a;
aux2=*a;

	//Caso seja o primeiro
	
	if(aux==NULL){
		novop = (LIST *)malloc(sizeof(LIST));
			if(novop == NULL) exit(-2);
		
		novop->value_length=value_len;

		novop->value=(char *)malloc((value_len)*sizeof(char));

		strcpy(novop->value,value);
		novop->key=key;
		novop->next=NULL;
		novop->ant=NULL;

		aux=novop;
		pthread_mutex_lock(&mux);
		*a=aux;
		pthread_mutex_unlock(&mux);
		return a;
	}
	

	printf("nao é o primeiro na lista\n");
	//verificar se ja existe	
	//
	aux2=*a;
	
	pthread_mutex_lock(&mux);
	
	while(aux2!=NULL){
			if(aux2->key == key){
				printf("valor já existente,acesso invalido\n");
				pthread_mutex_unlock(&mux);
				return a;
			}
			pthread_mutex_lock(&mux2);
			aux2=aux2->next;
			pthread_mutex_unlock(&mux2);
		}
	pthread_mutex_unlock(&mux);


	
	//Caso nao seja o primeiro
	
	novop = (LIST *)malloc(sizeof(LIST));
		if(novop == NULL) exit(-2);

	novop->value_length=value_len;

	novop->value=(char *)malloc((value_len)*sizeof(char));

	strcpy(novop->value,value);
	novop->key=key;
	novop->next=NULL;
	novop->ant=NULL;
		if(aux->next!= NULL){
			pthread_mutex_lock(&mux3);
			while(aux->next!=NULL){
				aux=aux->next;
			}
			pthread_mutex_unlock(&mux3);
			pthread_mutex_lock(&mux);
			aux->next = novop;
			novop->ant=aux;
			pthread_mutex_unlock(&mux);
		
		}else{
			pthread_mutex_lock(&mux);
			aux->next = novop;
			novop->ant= aux;
			pthread_mutex_unlock(&mux);
		}
	return a;
}

/*************************************************************************************************************************/
//Elimina toda a lista
void matalista(LIST** a){

LIST* aux;
LIST* prox;

	aux=*a;
pthread_mutex_lock(&mux);
	while(aux!=NULL){
		prox=aux->next;
		free(aux->value);
		free(aux);
		aux=prox;
	}
pthread_mutex_unlock(&mux);
	*a=aux;
	printf("Todos os items foram removidos\n");
return;
}
/*************************************************************************************************************************/
char* procura(LIST* a,uint32_t key){

	LIST* aux;

	aux=a;
	int o=0;
	pthread_mutex_lock(&mux);
		while(aux!=NULL){
			if(aux->key==key){
				o=1;
				pthread_mutex_unlock(&mux);
				return aux->value;
			}
			aux=aux->next;
		}
	pthread_mutex_unlock(&mux);
		if(o==0){
			printf("Nao existe nenhum valor para a chave: %u\n",key);
			aux=NULL;
		}
		return "\0";
}
/************ Modifica um elemento da lista********************************************************************************************************/
LIST** altera(LIST** a,uint32_t key,char* value,uint32_t value_len){

	LIST* aux;
	aux=*a;
	int o=0;
	pthread_mutex_lock(&mux);
		while(aux!=NULL){
			if(aux->key==key){
				o=1;
				pthread_mutex_lock(&mux2);
				aux->key=key;
				aux->value=realloc(aux->value,sizeof(char)*value_len);
				bzero(aux->value,value_len);
				strcpy(aux->value,value);
				pthread_mutex_unlock(&mux2);
				pthread_mutex_unlock(&mux);
				return a;
			}
			aux=aux->next;
		}
	pthread_mutex_unlock(&mux);
		if(o==0){
			printf("Nao existe nenhum valor para a chave: %u\n",key);
			aux=NULL;
		}
		return a;
}








/***************************************************************************************************************************/
void imprimeList(LIST* a){
LIST* aux;

	aux=a;
	if(aux==NULL){
		printf("LISTA VAZIA\n");
	}
pthread_mutex_lock(&mux);
	while(aux!=NULL){
	printf("Chave: %u Valor: %s\n",aux->key,aux->value);
	
	aux=aux->next;
	}
pthread_mutex_unlock(&mux);

return;
}
/*************************************************************************************************************************/
void eliminar(LIST** a,uint32_t key){

	LIST* aux=NULL;
	LIST* ant=NULL;
	//char* buffer;
	LIST* prox=NULL;

	if(a==NULL){
		printf("Valor Removido\n");
		return;
	}
	
	aux=*a;
	while(aux!=NULL){
		if(aux->key==key){
				if(aux->ant==NULL){ //VER SE E O 1o
					prox=aux->next;
						if(prox==NULL){ //SE FOR O UNICO ELEMENTO DA LISTA
							free(aux->value);
							free(aux);
							*a=prox;
							printf("Valor Removido\n");
							return;
						}
					prox->ant=NULL;
					free(aux->value);
					free(aux);
					*a=prox;
					printf("Valor Removido\n");
					return;
				}
				if(aux->next==NULL){ //VER SE E O ULTIMO
					ant=aux->ant;
					ant->next=NULL;
					free(aux->value);
					free(aux);
					printf("Valor Removido\n");
					return;
				}
				else{ //E UM PARA O MEIO
				
					ant=aux->ant;
					prox=aux->next;
					ant->next=prox;
					prox->ant=ant;
					free(aux->value);
					free(aux);
					printf("Valor Removido\n");
					return;
				}
		}
		aux=aux->next;
	}
	printf("Nao existe valor para a chave: %u\n",key);
return;
}
/*************************************************************************************************************************/
void commandList(LIST** lista){

uint32_t key = 0;

uint32_t value_len = 128;

char* value;

value=(char *)malloc(value_len*sizeof(char));

key=1;
strcpy(value,"Mekie Bitches!");

lista=novalor(lista, key,value,value_len);

key=2;
bzero(value,128);
strcpy(value,"SUP!");

lista=novalor(lista, key,value,value_len);

imprimeList(*lista);

key=3;
bzero(value,128);
strcpy(value,"Ligthning Strike!");

lista=novalor(lista, key,value,value_len);

imprimeList(*lista);

key=4;
bzero(value,128);
strcpy(value,"Jace!");

lista=novalor(lista, key,value,value_len);

imprimeList(*lista);

key=2;
eliminar(lista,key);

imprimeList(*lista);

matalista(lista);

free(value);
return;
}




