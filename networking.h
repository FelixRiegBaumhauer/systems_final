#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 1000

void error_check( int i, char *s );

int server_setup();
int server_connect(int sd);

int client_connect( char *host );

int send_data(int connection_descript,void *data);
int receive_data(int connection_descript,void *data);

struct game_info {
	int action;
	int amILeader;
	char username[64];
	char gamename[64];
	char password[64];
};

#endif
