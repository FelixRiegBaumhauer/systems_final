#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

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
	FILE * fd;
	char * string_to_write = calloc(1024,sizeof(char));
	char * file = "games.txt";
	
	strcat(string_to_write,username);
	strcat(string_to_write,",");
	strcat(string_to_write,gamename);
	strcat(string_to_write,",");
	strcat(string_to_write,password);
	strcat(string_to_write,",");
	char * rstr;
	sprintf(rstr,"%d",r);
	strcat(string_to_write,rstr);
	strcat(string_to_write,"\n");
	
	fd = fopen(file, "a+");
	
	if (fd == NULL) {
		printf("[file opening] error %d: %s\n", errno, strerror(errno) );
	}
	fwrite(string_to_write,sizeof(char),strlen(string_to_write),fd);
	fclose(fd);
}

int get_group_num(char * gamename) {
	FILE * fd;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;
	char * curGame;
	char * groupNum;
	
	fd = fopen("games.txt", "r+");
	
	while ((read = getline(&line, &len, fd)) != -1) {
		curGame = strchr(line,',');
		curGame++;
		if (strncmp(gamename,curGame,strlen(gamename)) == 0) {
			groupNum = strchr(curGame,',');
			groupNum = strchr(curGame,',');
			groupNum++;
			return atoi(groupNum);
		}
		count++;
	}
	return -1;
}

void process(sd) {
	struct game_info gminfo;
	int myGroup = 0;
	int amILeader = 0;
	
	int shmid;
	struct shmid_ds shmid_stuff;
	char * shm = calloc(1024,sizeof(char));
	
	int receive1 = read(sd,&gminfo.action,sizeof(int));
	int receive2 = read(sd,&gminfo.username,64);
	int receive3 = read(sd,&gminfo.gamename,64);
	int receive4 = read(sd,&gminfo.password,64);
	int receive5 = read(sd,&amILeader,sizeof(int));
	
	char ret[128];
	printf("a:%d\n",gminfo.action);
	printf("u:%s\n",gminfo.username);
	printf("g:%s\n",gminfo.gamename);
	printf("p:%s\n",gminfo.password);
	sprintf(ret,"%d,%s,%s,%s\n",gminfo.action,gminfo.username,gminfo.gamename,gminfo.password);
	write(sd,&ret,sizeof(ret));
	
	if (gminfo.action == 0) {
		srand(time(NULL));
		myGroup = rand();
		create_game(gminfo.username,gminfo.gamename,gminfo.password,myGroup);
	} else {
		myGroup = get_group_num(gminfo.gamename);
	}
	shmid = shmget(ftok("makefile", myGroup), 1024, IPC_CREAT|0644);
	
	shm = shmat(shmid,0,0);
	
	if (amILeader) {
		int waiting = read(sd,0,sizeof(int));
		strcpy(shm,"go");
	} else {
		while (strcmp(shm,"go") != 0) {}
		int go = 1;
		int going = write(sd,&go,sizeof(int));
	}
}