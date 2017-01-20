#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "networking.h"

//returns username
char * ask_for_handle() {
	printf("Please type the username you would like to use (max 12 characters) and press enter: ");
	char * buffer = calloc(12,sizeof(char));
  	fgets(buffer, 12, stdin);
	char * newline = strchr(buffer,'\n');
	*newline = 0;
	return buffer;
}

//returns 0 (create) or 1 (join)
int ask_for_action() {
	int ret;
	char * buffer = calloc(12,sizeof(char));
	while (strcmp(buffer,"c") != 0 && strcmp(buffer,"j") != 0) {
		printf("Please type 'c' to create a new game, or 'j' to join an existing game and press enter: ");
		fgets(buffer,12,stdin);
		char * newline = strchr(buffer,'\n');
		*newline = 0;
	}
	if (strcmp(buffer,"c") == 0) {
		return 0;
	}
	return 1;
}

//returns for name of game so others can identify it
char * ask_for_game_name() {
	printf("Please type the name you would like to use for others to identify your lobby and press enter: ");
	char * buffer = calloc(12,sizeof(char));
  	fgets(buffer, 12, stdin);
	char * newline = strchr(buffer,'\n');
	*newline = 0;
	return buffer;
}

//returns 0 (private) or 1 (public)
int ask_for_privacy_mode() {
	int ret;
	char * buffer = calloc(12,sizeof(char));
	while (strcmp(buffer,"private") != 0 && strcmp(buffer,"public") != 0) {
		printf("Please type 'public' to let anyone join or 'private' to create a password and press enter: ");
		fgets(buffer,12,stdin);
		char * newline = strchr(buffer,'\n');
		*newline = 0;
	}
	if (strcmp(buffer,"private") == 0) {
		return 0;
	}
	return 1;
}

//returns password
char * ask_for_password() {
	printf("Please type the password you would like to use for others to connect to your lobby and press enter: ");
	char * buffer = calloc(12,sizeof(char));
  	fgets(buffer, 12, stdin);
	char * newline = strchr(buffer,'\n');
	*newline = 0;
	return buffer;
}

char * join_game(char * lobbies) {
	if (strcmp(lobbies,"No games exist. Please create one.\n") == 0) {
		exit(0);
	} else {
		printf("%s\n",lobbies);
		printf("Please type the name of the lobby you would like to join and press enter: ");
		char * buffer = calloc(12,sizeof(char));
		fgets(buffer, 12, stdin);
		char * newline = strchr(buffer,'\n');
		*newline = 0;
		int notconnected = 1;
		while (notconnected) {
			//make sure the name they give is an actual name
			char * comma = strchr(lobbies,',');
			comma = 0;
			char * currentName;
			strcpy(currentName,lobbies);
			if (strcmp(currentName,buffer) == 0) {
				notconnected = 0;
			}
			*comma = ',';
			lobbies = strchr(lobbies,'\n');
		}
		return buffer;
	}
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

int main() {
	int action;
	char * username;
	char * gamename;
	char * password;

	int sd = client_connect("127.0.0.1");
	
	struct game_info gminfo;
	
	username = ask_for_handle();
	action = ask_for_action();
	
	if (action == 0) {
		gamename = ask_for_game_name();
		int privacymode = ask_for_privacy_mode();
		password = "";
		if (privacymode == 0) {
			password = ask_for_password();
		}
	} else {
		char * gamenames = get_current_lobbies();
		gamename = join_game(gamenames);
		password = ask_for_password();
	}
	
	gminfo.action = action;
	gminfo.username = username;
	gminfo.gamename = gamename;
	gminfo.password = password;
	
	send_data(sd,&gminfo);
	
	char * success_msg;
	receive_data(sd,&success_msg);
	printf("%s\n",success_msg);
}