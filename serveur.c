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

int totaldonnees=0;
int state =0;
char *login =NULL;
int isExist(char*,int);
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
	char buf[100]={'\0'},final[150];
	const char ch[1] ="\0";
	char *tok=NULL;
	char *dest=NULL;
	while((n=read(sockfd,buf,500))>0)
	{
		printf("n = %d\n",n);
		printf("BUF ------- > %s\n",buf);
		tok = malloc(sizeof(char) * n);
		tok = strtok(buf,ch);
		printf("\n\ntoken -> %s taille -> %d\n",tok,(int)strlen(tok));
		printf("socketFD -> %d\n",sockfd);
		printf("TOK ------- > %s\n",buf);
		dest = malloc(sizeof(char) * (strlen(tok)));
		strncpy(dest,tok,strlen(tok)-1);
		if(tok != NULL){
			printf("tok est alloué \n");
			tok = NULL;
		}
		printf("\nstrcpy -> dest -> %s taille-> %d\n",dest,(int)strlen(dest));
		if(tok !=NULL && dest !=NULL){
			if(strcmp(dest,"BONJ") == 0 && state == 0){
				printf("client a envoye - -%s\n",buf);
				write(sockfd,"WHO",n);
				++state;
			}  
			else if(state == 1){
				printf("client a envoye - - - %s\n",dest);
				login = malloc(sizeof(char) * (strlen(dest)));
				login = dest;
				write(sockfd,"PASSWD",n);
				++state;
			} 
			else if(state == 2){
				printf("client a envoye - - - %s\n",dest);
				login = malloc(sizeof(char) * (strlen(dest) + 1));
				strcat(login," ");
				strcat(login,dest);
				state = isExist(login,state);
				write(sockfd,"PASSWD",n); 
			} else {
				buf[n]='\0';
				printf("envoi dans socket de ? ? ? ? \n");
				write(sockfd,"? ? ? ?",n);
			}
		} else {
			printf("ERREUR");
		}
		printf("state -> %d",state);		
	}
	if(dest !=NULL){
		free(dest);
	}
	printf("total données reçues: %d\n",totaldonnees);
}

int isExist(char *login,int state) {
		int fd;
		int n=0;
		char buf[100]="{\0}";
		fd=open("log.txt",O_RDONLY);
		if (fd<0) {
			printf("Erreur d'ouverture du fichier\n");
			exit(-1);
		} else {
			printf("Log : %s",login);
			while((n=read(fd,buf,100))>0) {
				buf[n]='\0';
				printf("buf => %s\n",buf);
				//write(sockfd,buf,n);
			}
		}
		return state =0;
	}
