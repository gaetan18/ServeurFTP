#include <stdio.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "tools.c"

bool commandLocal(char* command) {

  if (strcmp(command,"ls") == 0) { //Verif LS
    return 1;
  }else if (strcmp(command,"pwd") == 0) { //PWD
    return 1;
  }else if (strcmp(command,"cd") == 0) { //CD 
    return 1;
  }else if (strcmp(command,"rm") == 0) { //RM
  	return 1;
  }
  return 0;
}


void executCommandLocal(char* command){


  int pid=0;

  if (( pid=fork()) == 0) {

    if (strcmp(command,"ls") == 0) { //LS

      execlp(command,command,NULL);

    }else if (strcmp(command,"pwd") == 0) { //PWD

      execlp(command,command,NULL);

    }else if (strcmp(command,"rm") == 0) { //RM
      printf("Quel  fichier à supprimer : ");
      	char file[50];
		    writeMessage(file);

		if(unlink(file) == 0) {
			printf("Fichier %s supprimé !\n", file);
		} else {
			printf("Fichier %s non supprimé !\n", file);
		}
    }else if (strcmp(command,"cd") == 0) { //CD
    	char path[50];
      	printf("Quel Répertoire souhaitait vous allez ? : ");
		    writeMessage(path);
      	chdir(path);

    }
  }

  wait(&pid);
}







