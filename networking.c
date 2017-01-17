#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int connection_descriptor;

int server_connect() {
  int sd;
  int connection;
  char buffer[200];

  sd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = INADDR_ANY;
  sock.sin_port = htons(9001);

  bind(sd, (struct sockaddr *)&sock, sizeof(sock));

  listen(sd, 1);

  struct sockaddr_in sock1;
  unsigned int socklen = sizeof(sock1);
  connection = accept(sd, (struct sockaddr *)&sock1, (socklen_t *)&socklen);

  //now connection can be used like a file descriptor
  connection_descriptor = connection;
  return connection;
}

int client_connect(char *serverip) {
  int sd;
  char *host = serverip;

  sd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  inet_aton(host, &sock.sin_addr);
  sock.sin_port = htons(9001);

  //bind(sd, (struct sockaddr *)&sock, sizeof(sock));
  //you no longer have to use bind on client side fo some reason
  connect(sd, (struct sockaddr *)&sock, sizeof(sock));

  //now you can use sd like a file descriptor
  connection_descriptor = sd;
  return sd;
}

int send_data(void *data) {
  int success = write(connection_descriptor, data, sizeof(data));
  return success;
}

void * receive_data() {
  void * data;
  int success = read(connection_descriptor, data, 60000);
  return data;
}

/*
int client_send(int connection, void * data) {
  int success = write(connection, data, sizeof(data));
  return success;
}

void * client_receive(int connection) {
  void * data;
  int success = read(connection, data, 60000);
  return data;
}*/
