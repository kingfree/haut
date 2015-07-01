#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

int do_server() {
  int fd;
  struct sockaddr_in server_addr;

  fd = socket(AF_INET, SOCK_STREAM, 0);

  if (fd == -1) {
    perror("error socket server");
    exit(1);
  }

  bzero((char*) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(9600);

  if (bind(fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("error binding");
    exit(1);
  }

  return fd;
}

void* do_child_thread(void* unused) {
  struct sockaddr_in client_addr;
  int fd;

  fd = socket(AF_INET, SOCK_STREAM, 0);

  if (fd == -1) {
    perror("error socket client");
    exit(1);
  }

  bzero((char*) &client_addr, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = INADDR_ANY;
  client_addr.sin_port = htons(9600);

  if (connect(fd, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
    perror("error connect");
    exit(1);
  }

  fprintf(stderr, "closing client socket\n");

  if (close(fd) < 0) {
    perror("error close client socket");
    exit(1);
  }

  fprintf(stderr, "closed client socket\n");

  return NULL;
}

int main(int argc, char** argv) {
  int server_fd, client_fd;
  socklen_t client_len;
  struct sockaddr_in client_addr;
  char buf[] = { 'a', '\n' };
  pthread_t child_thread;
  int rc;

  signal(SIGPIPE, SIG_IGN);

  server_fd = do_server();
  rc = listen(server_fd, 5);
  if (rc < 0) {
    perror("error listen");
    return 1;
  }

  rc = pthread_create(&child_thread, NULL, do_child_thread, NULL);
  if (rc != 0) {
    perror("error pthread_create");
    return 1;
  }

  client_len = sizeof(client_addr);
  client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_len);
  if (client_fd < 0) {
    perror("error accept");
    return 1;
  }

  while (1) {
    fprintf(stderr, "before send\n");
    rc = send(client_fd, buf, sizeof(buf), 0);
    fprintf(stderr, "after send: %d\n", rc);

    if (rc < 0) {
      if (errno == EPIPE) {
        break;
      } else {
        int so_type;
        socklen_t so_len = sizeof(so_type);
        getsockopt(client_fd, SOL_SOCKET, SO_TYPE, &so_type, &so_len);
        fprintf(stderr, "type: %d %d\n", so_type, SOCK_STREAM);

        perror("error send");
        return 1;
      }
    }
  }

  fprintf(stderr, "before server closing client fd\n");
  if (close(client_fd) < 0) {
    perror("error close client");
    return 1;
  }
  fprintf(stderr, "after server closing client fd\n");


  fprintf(stderr, "before server closing fd\n");
  if (close(server_fd) < 0) {
    perror("error close server");
    return 1;
  }
  fprintf(stderr, "after server closing fd\n");

  rc = pthread_join(child_thread, NULL);
  if (rc != 0 && rc != ESRCH) {
    fprintf(stderr, "error pthread_join: %d\n", rc);
    return 1;
  }

  return 0;
}
