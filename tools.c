
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


void writeMessage(char* message) {
	scanf("%[^\n]%*c", message);
}

bool commandServer(char* command) {

  if (strcmp(command,"rls") == 0) { //Verif LS
    return 1;
  }else if (strcmp(command,"rpwd") == 0) { //PWD
    return 1;
  }else if (strcmp(command,"rcd") == 0) { //CD 
    return 1;
  }else if (strcmp(command,"rm") == 0) { //RM
  	return 1;
  }
  return 0;
}
