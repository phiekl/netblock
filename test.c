#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#ifdef __linux__
#include <linux/netlink.h>
#endif

int test_connect();
int test_dns_functions();
int test_getnameinfo_and_gethostbyaddr();

int main() {
  test_connect();
  test_dns_functions();
  test_getnameinfo_and_gethostbyaddr();
  return 0;
}

int test_connect() {
  int sockfd;
  struct sockaddr_in addr_in;
  struct sockaddr_in6 addr_in6;
  struct sockaddr_un addr_un;
  struct sockaddr_nl addr_nl;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons(80);
  inet_pton(AF_INET, "192.0.2.1", &addr_in.sin_addr);
  connect(sockfd, (struct sockaddr *)&addr_in, sizeof(addr_in));
  close(sockfd);

  sockfd = socket(AF_INET6, SOCK_STREAM, 0);
  addr_in6.sin6_family = AF_INET6;
  addr_in6.sin6_port = htons(80);
  inet_pton(AF_INET6, "2001:db8::", &addr_in6.sin6_addr);
  connect(sockfd, (struct sockaddr *)&addr_in6, sizeof(addr_in6));
  close(sockfd);

  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  addr_un.sun_family = AF_UNIX;
  strcpy(addr_un.sun_path, "/tmp/demo_socket");
  connect(sockfd, (struct sockaddr *)&addr_un, sizeof(addr_un));
  close(sockfd);

#ifdef __linux__
  sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
  memset(&addr_nl, 0, sizeof(addr_nl));
  addr_nl.nl_family = AF_NETLINK;
  addr_nl.nl_pid = getpid();
  addr_nl.nl_groups = 0;
  connect(sockfd, (struct sockaddr *)&addr_nl, sizeof(addr_nl));
  close(sockfd);
#endif

  return 0;
}

int test_dns_functions() {
  struct addrinfo hints, *res;
  unsigned char answer[512];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // Both IPv4 and IPv6
  getaddrinfo("example.com", NULL, &hints, &res);
  freeaddrinfo(res);

  res_query("example.com", ns_c_in, ns_t_a, answer, sizeof(answer));
  res_search("example.com", ns_c_in, ns_t_a, answer, sizeof(answer));

  gethostbyname("example.com");
  gethostbyname2("example.com", AF_INET6);

  return 0;
}

int test_getnameinfo_and_gethostbyaddr() {
  char host[1024], service[20];
  struct sockaddr_in sa_in;
  struct sockaddr_in6 sa_in6;

  sa_in.sin_family = AF_INET;
  inet_pton(AF_INET, "192.0.2.1", &sa_in.sin_addr);
  getnameinfo((struct sockaddr *)&sa_in, sizeof(sa_in), host, sizeof(host),
              service, sizeof(service), 0);

  sa_in6.sin6_family = AF_INET6;
  inet_pton(AF_INET6, "2001:db8::", &sa_in6.sin6_addr);
  getnameinfo((struct sockaddr *)&sa_in6, sizeof(sa_in6), host, sizeof(host),
              service, sizeof(service), 0);

  gethostbyaddr(&sa_in.sin_addr, sizeof(sa_in.sin_addr), AF_INET);
  gethostbyaddr(&sa_in6.sin6_addr, sizeof(sa_in6.sin6_addr), AF_INET6);

  return 0;
}
