#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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
	
	fd = fopen("games.csv", "a");
	
	fwrite(string_to_write,sizeof(char),strlen(string_to_write),fd);
}

void process(sd) {
	struct game_info gminfo;
	int myGroup = 0;
	int r;
	
	int receive1 = read(sd,&gminfo.action,sizeof(int));
	int receive2 = read(sd,&gminfo.username,64);
	int receive3 = read(sd,&gminfo.gamename,64);
	int receive4 = read(sd,&gminfo.password,64);
	
	char ret[128];
	printf("a:%d\n",gminfo.action);
	printf("u:%s\n",gminfo.username);
	printf("g:%s\n",gminfo.gamename);
	printf("p:%s\n",gminfo.password);
	sprintf(ret,"%d,%s,%s,%s\n",gminfo.action,gminfo.username,gminfo.gamename,gminfo.password);
	write(sd,&ret,sizeof(ret));
	
	if (gminfo.action == 0) {
		srand(time(NULL));
		r = rand();
		create_game(gminfo.username,gminfo.gamename,gminfo.password,r);
		myGroup = r;
	} else {
		r = 0; //find the random number of the group and set my group equal to it
		myGroup = 0;
	}
	write(sd,&r,sizeof(r));
}