#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process();
void sub_server( int sd );
char * get_current_lobbies();
void create_game(char * username, char * gamename, char * password);

int main() {

  int sd, connection;

  sd = server_setup();
    
  while (1) {

    connection = server_connect( sd );

    int f = fork();
    if ( f == 0 ) {

      close(sd);
      sub_server(connection);

      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}

void sub_server(connection) {
	process(connection);
}

void process(sd) {
	struct game_info gminfo;
	
	int receive1 = read(sd,&gminfo.action,sizeof(int));
	printf("hi");
	int receive2 = read(sd,&gminfo.username,64);
	printf("hi");
	int receive3 = read(sd,&gminfo.gamename,64);
	printf("hi");
	int receive4 = read(sd,&gminfo.password,64);
	printf("hi");
	
	char ret[64];
	printf("a:%d\n",gminfo.action);
	printf("u:%s\n",gminfo.username);
	printf("g:%s\n",gminfo.gamename);
	printf("p:%s\n",gminfo.password);
	sprintf(ret,"%d,%s,%s,%s\n",gminfo.action,gminfo.username,gminfo.gamename,gminfo.password);
	write(sd,&ret,sizeof(ret));
}