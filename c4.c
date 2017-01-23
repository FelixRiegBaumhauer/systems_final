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

void ask_for_ready() {
	printf("Type anything when you're ready to start the game.");
	char * buffer = calloc(12,sizeof(char));
  	fgets(buffer, 12, stdin);
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
		/*int notconnected = 1;
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
		}*/
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

/*FUNCTIONS NEEDED:
-ignores apostrophe in buffer and implements that in all the above functions
-inside of the join_game() function, we need something that checks if the game exists at all
-inside get_username() check if username already exists if it does just reask until it doesnt
*/

int main() {
	int action;
	char * username;
	char * gamename;
	char * password;
	
	int amILeader;

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
		amILeader = 1;
	} else {
		char * gamenames = get_current_lobbies();
		gamename = join_game(gamenames);
		password = ask_for_password();
		amILeader = 0;
	}
	
	gminfo.action = action;
	strcpy(gminfo.username,username);
	strcpy(gminfo.gamename,gamename);
	strcpy(gminfo.password,password);
	
	printf("%d,%s,%s,%s\n",gminfo.action,gminfo.username,gminfo.gamename,gminfo.password);
	
	int sent1 = write(sd, &action, sizeof(action));
	int sent2 = write(sd, &gminfo.username, sizeof(gminfo.username));
	int sent3 = write(sd, &gminfo.gamename, sizeof(gminfo.gamename));
	int sent4 = write(sd, &gminfo.password, sizeof(gminfo.password));
	
	char success_msg[64];
	int receive = read(sd,&success_msg,sizeof(success_msg));
	printf("success msg: %s\n",success_msg);
	
	if (amILeader) {
		ask_for_ready();
	} else {
		printf("Waiting on the lobby leader to start the game.\n");
	}
	
	
}