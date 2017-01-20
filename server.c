#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

char * process();
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
  /*char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    send_data( sd, buffer);    
  }*/
  
}
char * process(sd) {
	struct game_info gminfo;
	gminfo.action = 0;
	gminfo.username = calloc(64,sizeof(char));
	gminfo.gamename = calloc(64,sizeof(char));
	gminfo.password = calloc(64,sizeof(char));
	receive_data(sd,&gminfo);
	
	char * ret;
	printf("a:%d\n",gminfo.action);
	printf("u:%s\n",gminfo.username);
	printf("g:%s\n",gminfo.gamename);
	printf("p:%s\n",gminfo.password);
	sprintf(ret,"%d,%s,%s,%s\n",gminfo.action,gminfo.username,gminfo.gamename,gminfo.password);
	send_data(sd,&ret);
	return ret;
}