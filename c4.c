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

int main() {
	int sd = client_connect("127.0.0.1");
	char * gamename;
	char * username = ask_for_handle();
	int action = ask_for_action();
	if (action == 0) {
		gamename = ask_for_game_name();
		int privacymode = ask_for_privacy_mode();
		char * password = "";
		if (privacymode == 0) {
			char * password = ask_for_password();
		}
		send_data(sd,&action);
		send_data(sd,username);
		send_data(sd,gamename);
		send_data(sd,password);
		char * success_msg;
		receive_data(sd,success_msg);
		printf("%s",success_msg);
	} else {
		send_data(sd,&action);
		//wait until you receive gamenames back
		char * gamenames;
		receive_data(sd,gamenames);
		char * game_to_join = join_game(gamenames);
		send_data(sd,game_to_join);
		send_data(sd,username);
		send_data(sd,game_to_join);
		char * password = "";
		send_data(sd,password);
		char * success_msg;
		receive_data(sd,success_msg);
		printf("%s",success_msg);
	}
}