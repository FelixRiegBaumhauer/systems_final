#include <sys/socket.h>
#include <sys/types.h>

int server_connect() {
  int sd;
  int connection;
  char buffer[200];

  sd = socket = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = INADDR_ANY;
  sock.sin_port = htons(9001);

  bind(sd, (struct sockaddr *)&sock, sizeof(sock));

  listen(sd, 1);

  struct sockaddr_in sock1;
  unsigned int socklen = sizeof(sock1);
  int connection = accept(sd, sock1, socklen);

  //now connection can be used like a file descriptor

  read(connection, buffer, sizeof(buffer));

  return connection;
}

int client_connect(char *serverip) {
  int sd;
  char *host = serverip;

  sd = socket(AF_INET, SOCK_STREAM, 0);

  struct sock_addr_in sock;
  sock.sin_family = AF_INET;
  inet_aton(host, &sock.sin_addr);
  sock.sin_port = htons(9001);

  //bind(sd, (struct sockaddr *)&sock, sizeof(sock));
  //you no longer have to use bind on client side fo some reason
  connect(sd, (struct sockaddr *)&sock, sizeof(sock));

  //now you can use sd like a file descriptor

  write(sd, buffer, sizeof(buffer));
  return 0;
}

int server_senddata(int connection, void *data) {
  int success = write(connection, data, sizeof(data);
  return success;
}

void * server_readdata(int connection) {
  void * data;
  int success = read(connection, data, 65536);
  return data;
}

int client_senddata(int connection, void data) {
  int success = write(connection, data, sizeof(data));
  return success;
}
