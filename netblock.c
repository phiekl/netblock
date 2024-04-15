#define _GNU_SOURCE
#include <arpa/inet.h>
#include <dlfcn.h>
#include <netdb.h>
#include <netinet/in.h>
#include <resolv.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  switch (addr->sa_family) {
  case AF_INET6: {
    char ip6[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)addr;
    inet_ntop(AF_INET6, &sin6->sin6_addr, ip6, INET6_ADDRSTRLEN);
    int port6 = ntohs(sin6->sin6_port);
    fprintf(stderr,
            "*** BLOCKED: connect(family=AF_INET6, addr=%s, port=%d) ***\n",
            ip6, port6);
    return -1;
  }
  case AF_INET: {
    struct sockaddr_in *sin = (struct sockaddr_in *)addr;
    char ip4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sin->sin_addr), ip4, INET_ADDRSTRLEN);
    int port4 = ntohs(sin->sin_port);
    fprintf(stderr,
            "*** BLOCKED: connect(family=AF_INET, addr=%s, port=%d) ***\n", ip4,
            port4);
    return -1;
  }
  #ifdef AF_NETLINK
  case AF_NETLINK:
    fprintf(stderr, "*** BLOCKED: connect(family=AF_NETLINK) ***\n");
    return -1;
  #endif
  case AF_UNIX: {
    struct sockaddr_un *sun = (struct sockaddr_un *)addr;
    fprintf(stderr, "*** BLOCKED: connect(family=AF_UNIX, addr=%s) ***\n",
            sun->sun_path);
    return -1;
  }
  #ifdef AF_UNSPEC
  case AF_UNSPEC:
    fprintf(stderr, "*** BLOCKED: connect(family=AF_UNSPEC) ***\n");
    return -1;
  #endif
  #ifdef AF_XDP
  case AF_XDP:
    fprintf(stderr, "*** BLOCKED: connect(family=AF_XDP) ***\n");
    return -1;
  #endif
  default:
    fprintf(stderr, "*** BLOCKED: connect(family=UNKNOWN) ***\n");
    return -1;
  }
}

int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints, struct addrinfo **res) {
  fprintf(stderr, "*** BLOCKED: getaddrinfo(%s) ***\n", node);
  *res = NULL;
  return EAI_FAIL;
}

int getnameinfo(const struct sockaddr *restrict sa, socklen_t salen,
                char *restrict host, socklen_t hostlen, char *restrict serv,
                socklen_t servlen, int flags) {
  char addrstr[INET6_ADDRSTRLEN];
  if (sa->sa_family == AF_INET) {
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;
    inet_ntop(AF_INET, &sin->sin_addr, addrstr, sizeof(addrstr));
  } else if (sa->sa_family == AF_INET6) {
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
    inet_ntop(AF_INET6, &sin6->sin6_addr, addrstr, sizeof(addrstr));
  } else {
    strncpy(addrstr, "*unknown*", sizeof(addrstr));
  }
  fprintf(stderr, "*** BLOCKED: getnameinfo(address=%s) ***\n", addrstr);
  return EAI_FAIL;
}

int res_query(const char *name, int class, int type, unsigned char *answer,
              int anslen) {
  fprintf(stderr, "*** BLOCKED: res_query(name=%s) ***\n", name);
  return -1;
}

int res_search(const char *name, int class, int type, unsigned char *answer,
               int anslen) {
  fprintf(stderr, "*** BLOCKED: res_search(name=%s) ***\n", name);
  return -1;
}

struct hostent *gethostbyname(const char *name) {
  fprintf(stderr, "*** BLOCKED: gethostbyname(name=%s) ***\n", name);
  h_errno = HOST_NOT_FOUND;
  return NULL;
}

struct hostent *gethostbyname2(const char *name, int af) {
  fprintf(stderr, "*** BLOCKED: gethostbyname2(name=%s) ***\n", name);
  h_errno = HOST_NOT_FOUND;
  return NULL;
}

struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type) {
  char addrstr[INET6_ADDRSTRLEN];
  if (type == AF_INET) {
    inet_ntop(AF_INET, addr, addrstr, sizeof(addrstr));
  } else if (type == AF_INET6) {
    inet_ntop(AF_INET6, addr, addrstr, sizeof(addrstr));
  } else {
    strncpy(addrstr, "*unknown*", sizeof(addrstr));
  }
  fprintf(stderr, "*** BLOCKED: gethostbyaddr(address=%s) ***\n", addrstr);
  h_errno = HOST_NOT_FOUND;
  return NULL;
}
