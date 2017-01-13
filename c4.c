#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

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
	
	fwrite(string_to_write,sizeof(string_to_write),sizeof(char),fd);
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

char * join_game() {
	char * lobbies = get_current_lobbies();
	if (strcmp(lobbies,"No games exist. Please create one.\n") == 0) {
		printf("%s\n",lobbies);
		return 0;
	} else {
		printf("%s\n",lobbies);
		printf("Please type the name of the lobby you would like to join and press enter: ");
		char * buffer = calloc(12,sizeof(char));
		fgets(buffer, 12, stdin);
		char * newline = strchr(buffer,'\n');
		*newline = 0;
		return buffer;
	}
}

int main() {
	char * username = ask_for_handle();
	int action = ask_for_action();
	if (action == 0) {
		char * gamename = ask_for_game_name();
		int privacymode = ask_for_privacy_mode();
		if (privacymode == 0) {
			char * password = ask_for_password();
			create_game(username,gamename,password);
		} else {
			create_game(username,gamename,"");
		}
	} else {
		join_game();
	}
}