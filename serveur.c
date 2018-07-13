#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include "functionServer.c"

int totaldonnees=0;
int state =0;
char *login =NULL;
char *identifiant =NULL;
char *pswd =NULL;
int err = 0;
void str_echo(int);
int main(int argc,char **argv)
{
int lfd,cfd;
pid_t childpid;
socklen_t	clilen;
struct sockaddr_in cliaddr,servaddr;
struct sigaction act;

if (argc!=2)
	{
	printf("Usage : serveur Port\n");
	exit(-1);
	}

act.sa_handler=SIG_DFL;
act.sa_flags=SA_NOCLDWAIT;
sigaction(SIGCHLD,&act,NULL);

lfd=socket(AF_INET,SOCK_STREAM,0);

bzero(&servaddr,sizeof(servaddr));

servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
servaddr.sin_port=htons(atoi(argv[1]));

bind(lfd,(struct sockaddr *) &servaddr, sizeof(servaddr));	
perror("bind");

listen(lfd,10);

for(;;)
	{
	clilen=sizeof(cliaddr);
	printf("cliaddr - -> %d",cliaddr.sin_addr.s_addr);
	if((cfd=accept(lfd,(struct sockaddr *) &cliaddr,&clilen))<0)
		{
		printf("erreur accept\n");
		exit(-1);
		}
	if((childpid=fork())==0)
		{
		close(lfd);
		str_echo(cfd);
		exit(0);
		}
	close(cfd);
}

}

void  str_echo(int sockfd)
{
	int n =0;
	char buf[500]={'\0'},final[150];
	const char ch[1] ="\0";
	char *tok=NULL;
	char *dest=NULL;
	printf("sockfd %d",sockfd);
	printf("buff %s",buf);
	int nbyte = strlen(buf);
	while((n=read(sockfd,buf,500))>0)
	{
		printf("n = %d\n",n);
		printf("BUF ------- > %s \n",buf);
		printf("LEN ---> %d \n",(int)strlen(buf));
		tok = malloc(sizeof(char) * n);
		tok = strtok(buf,ch);
		printf("\n\ntoken -> %s taille -> %d\n",tok,(int)strlen(tok));
		printf("socketFD -> %d\n",sockfd);
		printf("TOK ------- > %s\n",buf);
		dest = malloc(sizeof(char) * (strlen(tok)));
		strncpy(dest,tok,strlen(tok)-1);
		if(tok != NULL){
			printf("tok est alloué \n");
		}
		printf("\nstrcpy -> dest -> %s taille-> %d\n",dest,(int)strlen(dest));
		if(tok !=NULL && dest !=NULL){
			if(strcmp(dest,"BONJ") == 0 && state == 0){
				printf("client a envoye - -%s\n",buf);
				write(sockfd,"WHO",3);
				reinitialiseBuf(buf);
				++state;
			}  
			else if(state == 1){
				printf("client a envoye - - - %s\n",dest);
				identifiant = malloc(sizeof(char) * (strlen(dest)));
				identifiant = dest;
				write(sockfd,"PASSWD",6);
				reinitialiseBuf(buf);
				++state;
			} 
			else if(state == 2){
				printf("client a envoye - - - %s\n",dest);
				pswd = malloc(sizeof(char) * (strlen(dest) + 1));
				pswd = dest;
				login = malloc(sizeof(char) * strlen(identifiant) + strlen(identifiant));
				strcat(login,identifiant);
				strcat(login," ");
				strcat(login,pswd);
				printf("login apres ---->%s",login);
				printf("errrrrreur -- %d\n",err);
				err = isExist(login,state,err);
				printf("\nerr ------+ %d\n",err);
				if (err == -1){
					write(sockfd,"WELC",4);
					state++;
				}
				if(err>0 && err<3){
					write(sockfd,"ERRPASSWD",9);
				} 
				if (err == 3){
					write(sockfd,"BYE",3);
					err = 0;
					state =0;
				}
				reinitialiseBuf(buf);
			}
			else if(state == 3){
				//SOCKET2
			} else {
				printf("envoi dans socket de ? ? ? ? \n");
				write(sockfd,"? ? ? ?",7);
				reinitialiseBuf(buf);
			}
		} else {
			printf("ERREUR");
		}
		printf("state -> %d",state);		
	}
	if(dest !=NULL){
		free(dest);
	}
	if(identifiant !=NULL){
		free(identifiant);
	}
	if(pswd !=NULL){
		free(pswd);
	}
	if(login !=NULL){
		free(login);
	}
	reinitialiseBuf(buf);
	printf("total données reçues: %d\n",totaldonnees);
}