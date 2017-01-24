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

void initialize(char board[], int x, int y){
  int i;
  int j;
  for(i=0; i<y; i++){
    for(0; j<x; j++){
      board[(j+x*i)]='-';
    }
  }
}

char * get_board(char board[], int x, int y){
  char * ret = calloc(strlen(board),sizeof(char));
  int i;
  int j;
  for(i=(y-1); i>=0; i--){
    for(j=0; j<x; j++){
      strcat(ret,&board[j+x*i]);
    }
    strcat(ret,"\n");
  }

  strcat(ret,"------------------------------\n");
  return ret;
}


int placer(char board[], int x, int y, int target, char player_icon){
  if(target >= x){
    return -1;
  }
  
  int i;
  for(i=0; i<y; i++){
    if(board[target+(x*i)] == '-'){

      board[target+(x*i)] = player_icon;      
      
      return 1;//success
    }
  }
  return -1;//only if row full
}


int vertical_check(char board[], int x, int y, int xpos, int ypos, char symbol){
  int inc;
  int pos = (y*ypos) + xpos;

  int connect_number=4;//hence called connect four
  
  for(inc=0; inc<connect_number; inc++){
    if(board[pos+(inc*y)] != symbol){
      return -1;
    }
  }
  return 1;
}


int horizontal_check(char board[], int x, int y, int xpos, int ypos, char symbol){
  int inc;
  int pos = (y*ypos) + xpos;

  int connect_number=4;//hence called connect four

  //this happens if it is checking on the edge
  if( ((pos+3)/7) != (pos/7)){
    return -1;
  }
  
  for(inc=0; inc<connect_number; inc++){
    if(board[pos+inc] != symbol){
      return -1;
    }
  }
  return 1;
}

//right and up
int up_slant_check(char board[], int x, int y, int xpos, int ypos, char symbol){
  int inc;
  int pos = (y*ypos) + xpos;

  int connect_number=4;//hence called connect four

  if( ((pos/7)+3) != ((pos+3*(x+1))/7) ){
    return -1;
  }
  
  for(inc=0; inc<connect_number; inc++){
    if(board[pos+inc*(y+1)] != symbol){
      return -1;
    }
  }
  return 1;
}

//right and down
int down_slant_check(char board[], int x, int y, int xpos, int ypos, char symbol){
  int inc;
  int pos = (y*ypos) + xpos;

  int connect_number=4;//hence called connect four

  if( ((pos/7)+3) != ((pos+3*(x-1))/7) ){
    return -1;
  }
  
  for(inc=0; inc<connect_number; inc++){
    if(board[pos+inc*(y-1)] != symbol){
      return -1;
    }
  }
  return 1;
}

int checker(char board[], int x, int y, char symbol){
  int i;
  int j;
  for(i=0; i<y; i++){
    for(j=0; j<x; j++){
      if(vertical_check(board, x, y, j, i, symbol) == 1){
	return 1;
      }
      if(horizontal_check(board, x, y, j, i, symbol) == 1){
	return 1;
      }
      if(up_slant_check(board, x, y, j, i, symbol) == 1){
	return 1;
      }
      if(down_slant_check(board, x, y, j, i, symbol) == 1){
	return 1;
      }
    }
  }
  return -1;//if true
}

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

void process(int sd) {
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
	shm = (int *)shmat(shmid,0,0);
	
	shmid2 = shmget(ftok("makefile", myGroup+1), 1024, IPC_CREAT|0644);
	shm2 = (char *)shmat(shmid2,0,0);
	
	shmid3 = shmget(ftok("makefile", myGroup+2), 1024, IPC_CREAT|0644);
	shm3 = (char *)shmat(shmid3,0,0);
	
	strcpy(shm3,"asdf\n");
	
	if (gminfo.amILeader) {
		int waiting;
		while (waiting != 1) {
			read(sd,&waiting,sizeof(int));
		}
		strcpy(shm3,"go\n");
		char board[49];
		initialize(board,7,7);
		strcpy(shm2,board);
	} else {
		while (strcmp(shm3,"go\n") != 0) {}
		int go = 1;
		int going = write(sd,&go,sizeof(int));
	}

	char buffer[1024];
	char icons[10] = {'O','X','@','*','$','M','W'};
	while (checker(shm2,7,7,icons[0]) != -1 && checker(shm2,7,7,icons[1])) {
		while (*shm != gminfo.amILeader) {
		  strcpy(buffer,"Waiting for other player(s).\n");
		  write(sd,&buffer,sizeof(buffer));
		  sleep(5);
		}
		strcpy(buffer,get_board(shm2,7,7));
		write(sd,&buffer,sizeof(buffer));
		read(sd,&buffer,sizeof(buffer));
		if (*shm == 1) {
		  *shm = 0;
		} else {
		  *shm = 1;
		}
	}
}
