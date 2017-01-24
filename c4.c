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

char * ask_for_password_joining() {
	printf("Please type the password of the lobby: ");
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
		printf("No games exist. Please create one.\n");
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

char * make_move() {
  printf("Make a move.\n");
  char * buffer = calloc(1024,sizeof(char));
  fgets(buffer, 1024, stdin);
  return buffer;
}

//SERVER FUNCTION, games.csv IS A SERVER ONLY FILE --> need a function that asks the server
//to call this function, and then returns the result
char * get_current_lobbies() {
	int fd;
	int count = 0;
	char * gamenames = calloc(1024,sizeof(char));
	
	strcat(gamenames,"Current games:\n");
	
	fd = open("games.txt", O_RDWR);
	read(fd,gamenames,1024);
	
	return gamenames;
}

/*FUNCTIONS NEEDED:
-ignores apostrophe in buffer and implements that in all the above functions
-inside of the join_game() function, we need something that checks if the game exists at all
-inside get_username() check if username already exists if it does just reask until it doesnt
*/

char * stripApostrophes(char *input) {
  //printf("%s", input);
  char *final = (char *)calloc(1, 100);
  strcpy(final, input);
  //printf("%p", strchr(final, '\''));
  while(strchr(final, '\'')) {
    printf("run");
    char *loc = strchr(final, '\'');
    //printf("%s", loc);
    *loc = 0;
    loc++;
    strcat(final, loc);
  }
  return final;
  //return "asdf";
}

void print_board(char board[], int x, int y){
  int i;
  int j;
  for(i=(y-1); i>=0; i--){
    for(j=0; j<x; j++){
      printf("%c",board[j+x*i]);
    }
    printf("\n");
  }

  printf("-------------------------\n");
}

int main() {
  //char *lol = "i'm s'o w'e'a'k";
        //char *lolz = stripApostrophes(lol);
        //printf("%s", lolz);
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
		password = ask_for_password_joining();
		amILeader = 0;
	}
	
	gminfo.action = action;
	gminfo.amILeader = amILeader;
	strcpy(gminfo.username,username);
	strcpy(gminfo.gamename,gamename);
	strcpy(gminfo.password,password);
	
	int sent1 = write(sd, &action, sizeof(action));
	int sent2 = write(sd, &gminfo.username, sizeof(gminfo.username));
	int sent3 = write(sd, &gminfo.gamename, sizeof(gminfo.gamename));
	int sent4 = write(sd, &gminfo.password, sizeof(gminfo.password));
	int sent5 = write(sd, &amILeader, sizeof(amILeader));
	
	if (amILeader) {
		ask_for_ready();
		int go = 1;
		write(sd, &go, sizeof(int));
	} else {
		printf("Waiting on the lobby leader to start the game.\n");
		int getting; 
		while (getting != 1) {
			read(sd,&getting,sizeof(int));
		}
	}

	printf("Ready to go!\n");
	
	char buffer[1024];
	
	while (strcmp(buffer,"Game over!\n") != 0) {
	  while (strncmp(buffer,"0",1) != 0 && strncmp(buffer,"1",1 != 0)) {
	    read(sd,&buffer,sizeof(buffer));
	    sleep(1);
	  }
	  strcpy(buffer,strchr(buffer,'\n')+1);
      printf("buffer[0] %d", buffer[0]);
	  if (strncmp(buffer,"0",1)) {
	    if (gminfo.amILeader == 0) {
          print_board(buffer,7,7);
	      char * move = make_move();
	      strcpy(buffer,"move:");
	      strcat(buffer,move);
	      write(sd,&buffer,sizeof(buffer));
	    } else {
	      printf("Waiting for other player to move.\n");
	      while (1) {
            read(sd,&buffer,sizeof(buffer));
            if (strncmp(buffer,"1",1) == 0) {
              break;
            }
            sleep(1);
	      }
	    }
	  }
	  else if (strncmp(buffer,"1",1)) {
	    if (gminfo.amILeader == 1) {
          print_board(buffer,7,7);
	      char * move = make_move();
	      strcpy(buffer,"move:");
	      strcat(buffer,move);
	      write(sd,&buffer,sizeof(buffer));
	    } else {
	      printf("Waiting for other player to move.\n");
          while (1) {
            read(sd,&buffer,sizeof(buffer));
            if (strncmp(buffer,"0",1) == 0) {
              break;
            }
		sleep(1);
	      }
	    }
	  }
	}
	printf("%s\n",buffer);
	exit(0);
}
