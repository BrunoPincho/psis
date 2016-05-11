#include "threadserv.h"



/**************************************************************************************************************************/
//Cria um novo elemento na lista
LIST** novalor(LIST** a, uint32_t key,char* value,uint32_t value_len){

LIST* novop;
LIST* aux=NULL;

aux=*a;
	

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
		*a=aux;
		return a;
	}

	printf("nao é o primeiro na lista\n");
	//verificar se ja existe	
	aux=*a;
	while(aux!=NULL){
			if(aux->key==key){
				printf("valor já existente,acesso invalido\n");
				return a;
			}
			aux=aux->next;
		}



	
	//Caso nao seja o primeiro
		aux=*a;
	novop = (LIST *)malloc(sizeof(LIST));
		if(novop == NULL) exit(-2);

	novop->value_length=value_len;

	novop->value=(char *)malloc((value_len)*sizeof(char));

	strcpy(novop->value,value);
	novop->key=key;
	novop->next=NULL;
	novop->ant=NULL;
		if(aux->next!= NULL){
			while((aux->next!=NULL) ||(aux->key!=key)){
				aux=aux->next;
			}
			aux->next = novop;
			novop->ant=aux;
		}else{
	
			aux->next = novop;
			novop->ant= aux;

		}
	return a;
}

/*************************************************************************************************************************/
//Elimina toda a lista
void matalista(LIST** a){

LIST* aux;
LIST* prox;

	aux=*a;

	while(aux!=NULL){
		prox=aux->next;
		free(aux->value);
		free(aux);
		aux=prox;
	}
	*a=aux;
	printf("Todos os items foram removidos\n");
return;
}
/*************************************************************************************************************************/
char* procura(LIST* a,uint32_t key){

	LIST* aux;

	aux=a;
	int o=0;
		while(aux!=NULL){
			if(aux->key==key){
				o=1;
				return aux->value;
			}
			aux=aux->next;
		}
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
		while(aux!=NULL){
			if(aux->key==key){
				o=1;
				aux->key=key;
				aux->value=realloc(aux->value,sizeof(char)*value_len);
				bzero(aux->value,value_len);
				strcpy(aux->value,value);
				
				return a;
			}
			aux=aux->next;
		}
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

	while(aux!=NULL){
	printf("Chave: %u Valor: %s\n",aux->key,aux->value);
	
	aux=aux->next;
	}

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


/*************************************************************************************************************************/
/*
int main(){

LIST* lista=NULL;

commandList(&lista);


exit(0);
}*/


