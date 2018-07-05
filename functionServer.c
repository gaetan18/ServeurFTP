#include <stdio.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "tools.c"


bool commandServer(char* command) {

  if (strcmp(command,"rls") == 0) { //Verif LS
    return 1
  }else if (strcmp(command,"rpwd") == 0) { //PWD
    return 1;
  }else if (strcmp(command,"rcd") == 0) { //CD 
    return 1;
  }else if (strcmp(command,"rm") == 0) { //RM
  	return 1;
  }else{
  	printf("La Commande n'existe pas");
    return 0;
  }
}

void executCommandServer(char* command){
	

}