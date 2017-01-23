#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

#include "networking.h"

void process();
char * get_current_lobbies();
void create_game(char * username, char * gamename, char * password, int r);

int main() {
  int sd, connection;
  sd = server_setup();
  while (1) {
    connection = server_connect( sd );
    int f = fork();
    if ( f == 0 ) {
      close(sd);
      process(connection);

      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}

//SERVER FUNCTION, games.csv IS A SERVER ONLY FILE --> need a function that asks the server
//to call this function, and then returns the result
void create_game(char * username, char * gamename, char * password, int r) {
	int fd;
	char string_to_write[1024];
	
	strcat(string_to_write,username);
	strcat(string_to_write,",");
	strcat(string_to_write,gamename);
	strcat(string_to_write,",");
	strcat(string_to_write,password);
	strcat(string_to_write,",");
	char buffer[32];
	snprintf(buffer,sizeof(buffer),"%d",r);
	strcat(string_to_write,buffer);
	strcat(string_to_write,"\n");
	
	printf("str %s\n",string_to_write);
	
	fd = open("games.txt", O_APPEND);
	printf("fd: %d\n",fd);
	write(fd,"hello",strlen("hello"));
	write(fd,string_to_write,strlen(string_to_write));
	close(fd);
}

int get_group_num(char * gamename) {
	int fd;
	char * line = NULL;
	char * curGame;
	char * groupNum;
	char * fileContents;
	
	fd = open("games.txt", O_RDWR);
	read(fd,fileContents,1024);
	
	line = fileContents;
	
	while (line != NULL) {
		curGame = strchr(line,',');
		curGame++;
		if (strncmp(gamename,curGame,strlen(gamename)) == 0) {
			groupNum = strchr(curGame,',');
			groupNum = strchr(curGame,',');
			groupNum++;
			return atoi(groupNum);
		}
		line = strchr(line,'\n');
		line++;
	}
	close(fd);
	return -1;
}

void process(sd) {
	struct game_info gminfo;
	int myGroup = 0;
	
	int shmid;
	struct shmid_ds shmid_stuff;
	char * shm = calloc(1024,sizeof(char));
	
	int receive1 = read(sd,&gminfo.action,sizeof(int));
	int receive2 = read(sd,&gminfo.username,64);
	int receive3 = read(sd,&gminfo.gamename,64);
	int receive4 = read(sd,&gminfo.password,64);
	int receive5 = read(sd,&gminfo.amILeader,sizeof(int));
	
	if (gminfo.action == 0) {
		srand(time(NULL));
		myGroup = rand();
		printf("hello\n");
		create_game(gminfo.username,gminfo.gamename,gminfo.password,myGroup);
	} else {
		myGroup = get_group_num(gminfo.gamename);
	}
	shmid = shmget(ftok("makefile", myGroup), 1024, IPC_CREAT|0644);
	
	shm = shmat(shmid,0,0);
	
	if (gminfo.amILeader) {
		int waiting = read(sd,0,sizeof(int));
		strcpy(shm,"go");
	} else {
		while (strcmp(shm,"go") != 0) {}
		int go = 1;
		int going = write(sd,&go,sizeof(int));
	}
}