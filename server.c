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
#include "game.h"

void process(int sd);
//char * get_current_lobbies();
//void create_game(char * username, char * gamename, char * password, int r);

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
	int x;
	for(x=0; x<1024; x++){
	  string_to_write[x] = 0;
	}
	
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
	
	fd = open("games.txt", O_WRONLY | O_APPEND, 0666);
	char *  buff[100];
	write(fd,string_to_write,strlen(string_to_write));
	close(fd);
}

int get_group_num(char * gamename) {
	int fd;
	char * line = calloc(4096,sizeof(char));
	char * groupNum;
	char * curGame;
	
	fd = open("games.txt", O_RDWR);
	read(fd,line,4096);
	while (line != NULL) {
		curGame = strchr(line,',');
		curGame++;
		if (strncmp(gamename,curGame,strlen(gamename)) == 0) {
			groupNum = strchr(curGame,',');
			groupNum++;
			groupNum = strchr(groupNum,',');
			groupNum++;
			char * endNum = strchr(groupNum,'\n');
			endNum = 0;
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
	
	//store whose turn it is
	int shmid;
	struct shmid_ds shmid_stuff;
	int * shm;
	
	//store board
	int shmid2;
	struct shmid_ds shmid_stuff2;
	char * shm2 = calloc(1024,sizeof(char));
	
	//store num of players
	int shmid3;
	struct shmid_ds shmid_stuff3;
	char * shm3 = calloc(1024,sizeof(char));
	
	int receive1 = read(sd,&gminfo.action,sizeof(int));
	int receive2 = read(sd,&gminfo.username,64);
	int receive3 = read(sd,&gminfo.gamename,64);
	int receive4 = read(sd,&gminfo.password,64);
	int receive5 = read(sd,&gminfo.amILeader,sizeof(int));
	
	if (gminfo.action == 0) {
		srand(time(NULL));
		myGroup = rand();
		create_game(gminfo.username,gminfo.gamename,gminfo.password,myGroup);
	} else {
		myGroup = get_group_num(gminfo.gamename);
	}
	
	//1: leader, 0: not leader
	shmid = shmget(ftok("makefile", myGroup), 1024, IPC_CREAT|0644);
	shm = shmat(shmid,0,0);
	
	strcpy(shm,"asdf\n");
	
	shmid2 = shmget(ftok("makefile", myGroup+1), 1024, IPC_CREAT|0644);
	shm2 = shmat(shmid2,0,0);
	
	shmid3 = shmget(ftok("makefile", myGroup+2), 1024, IPC_CREAT|0644);
	shm3 = shmat(shmid3,0,0);
	
	if (gminfo.amILeader) {
		int waiting;
		while (waiting != 1) {
			read(sd,&waiting,sizeof(int));
		}
		strcpy(shm,"go\n");
		char board[49];
		initialize(board,7,7);
		strcpy(shm2,board);
	} else {
		while (strcmp(shm,"go\n") != 0) {}
		int go = 1;
		int going = write(sd,&go,sizeof(int));
	}
	
	while (checker(shm2,7,7) != -1) {
		while (*shm1 != amILeader) {
			time.sleep(5);
		}
	}
}
