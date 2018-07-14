#include <stdio.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "tools.c"


void executCommandServer(int client, char* command){
  printf("je suis dans executCommandServer\n");
  if (strcmp(command, "rcd") == 0) {
    char* question = "Quel Répertoire souhaitait vous allez ? ";
    write(client,"question",sizeof(question));
    while(1){
      read(client,command,500);
      if (strcmp(command, "rcd") !=0)
      {
        if (chdir(command))
        {
           write(client,"cd ok",5);
        }
        else{
          write(client,"cd nok",6);
        }

      }
    }
    
  }
  else if (strcmp(command, "rls") == 0 || strcmp(command, "rpwd") == 0)
  {
    /*printf("je suis dans executCommandServer pour : %s\n",command);
    char result[1000];*/
    int stdout_standard = dup(STDOUT_FILENO);
    /*int log_file = open("tmplog", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    dup2(log_file, STDOUT_FILENO);*/

    dup2(client, STDOUT_FILENO);

    int pid=fork();
    if (pid == 0){

      if (strcmp(command, "rls") == 0){
         execlp("ls", "ls", NULL);
      }
      if (strcmp(command, "rpwd") == 0){
         execlp("pwd", "pwd", NULL);
      }

    } else{
        wait(&pid);
    }

    dup2(stdout_standard, STDOUT_FILENO);
    close(stdout_standard);
    //close(log_file);

   
   //remove("tmplog");

  }
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