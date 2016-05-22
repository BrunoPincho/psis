#include "threadserv.h"


int main(){
	pid_t front_server;
	sum_trd=0;
	quit=0;
	
	sincro.valor=0;
	
	if((front_server = fork())==-1){
		perror("fork");
		exit(-1);
	}
	
	if(front_server == 0){
		
		Frontserver();
	}else{
			
			
	DataServer();
	}
	exit(1);
}
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		
		
		
		
		
		
