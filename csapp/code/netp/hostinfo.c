#include <stdio.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
  char **pp;
  struct in_addr addr;
  struct hostent *hostp;
  
  if (argc != 2) {
    fprintf(stderr, "用法: %s <域名或IP>\n", argv[0]);
    exit(0);
  }

  if (inet_aton(argv[1], &addr) != 0)
    hostp = gethostbyaddr((const char *) &addr, sizeof(addr), AF_INET);
  else
    hostp = gethostbyname(argv[1]);
  
  printf("主机名: %s\n", hostp->h_name);

  exit(0);
}
