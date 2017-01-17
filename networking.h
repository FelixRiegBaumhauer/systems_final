#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int server_connect();
int client_connect(char *serverip);
int send_data(void *data);
void * receive_data();
