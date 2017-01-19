#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
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
      sub_server(sd);

      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}


void sub_server(sd) {

	process(sd);

  /*char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    send_data( sd, buffer);    
  }*/
  
}
char * process(sd) {
	int * action;
	char * username;
	char * gamename;
	char * password;
	
	//1. receive action (0 for create, 1 for join)
	action = receive_data(sd);
	if (*action == 0) {
		receive_data(sd,username); //2. receive username
		receive_data(sd,gamename); //3. receive gamename
		receive_data(sd,password); //4. receive password
	} else {
		send_data(sd,get_current_lobbies()); //1a. 
		receive_data(sd,username);
		receive_data(sd,gamename);
		receive_data(sd,password);
	}
	char * ret;
	sprintf(ret,"%s,%s,%s,%s\n",action,username,gamename,password);
}

//SERVER FUNCTION, games.csv IS A SERVER ONLY FILE --> need a function that asks the server
//to call this function, and then returns the result
char * get_current_lobbies() {
	FILE * fd;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;
	char * gamenames = calloc(1024,sizeof(char));
	
	strcat(gamenames,"Current games:\n");
	
	fd = fopen("games.csv", "r");
	
	if (fd == NULL) {
		return "File not found.\n";
	}
	
	while ((read = getline(&line, &len, fd)) != -1) {
		strcat(gamenames,line);
		strcat(gamenames,"\n");
		count++;
	}
	
	if (count == 0) {
		return "No games exist. Please create one.\n";
	}
	
	fclose(fd);
	if (line) {
		free(line);
	}
	
	return gamenames;
}

//SERVER FUNCTION, games.csv IS A SERVER ONLY FILE --> need a function that asks the server
//to call this function, and then returns the result
void create_game(char * username, char * gamename, char * password) {
	FILE * fd;
	char * string_to_write = calloc(1024,sizeof(char));
	
	strcat(string_to_write,username);
	strcat(string_to_write,",");
	strcat(string_to_write,gamename);
	strcat(string_to_write,",");
	strcat(string_to_write,password);
	strcat(string_to_write,"\n");
	
	fd = fopen("games.csv", "a");
	
	fwrite(string_to_write,sizeof(char),strlen(string_to_write),fd);
}