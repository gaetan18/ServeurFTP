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
    return 1;
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


void reinitialiseBuf(char buf[500]) {
  for(int i=0; i<500; i++){
		buf[i]='\0';
	}
}

int isExist(char *login,int state, int err) {
	int fd;
	int n=0;
  char c;
	char line[100]={'\0'};
	char *line2 = NULL;
	int cptEnter = 0;
	fd=open("log.txt",O_RDONLY);
	if (fd<0) {
		printf("Erreur d'ouverture du fichier\n");
		exit(-1);
	} else {
		printf("Log : %s -> %d",login,(int)strlen(login));
		line2 = malloc(sizeof(char) * 1);
		while((n=read(fd,&c,1))>0) {
			if(c == '\n'){
				line2 = NULL;
				line2 = realloc(line2,sizeof(char) * cptEnter);
				strncpy(line2,line,cptEnter);
				printf("line2 %s - %d - login %s -%d \n",line2,(int)strlen(line2),login,(int)strlen(login));
				if(strcmp(line2,login) == 0){
					err = -1;
				}
				cptEnter=0;
			} else{
				line[cptEnter]=c;
				cptEnter++;
			}
		}
		free(line2);
	}
	if (err != -1){
		err ++;
  }
  return err;
}