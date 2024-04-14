# netblock

This library can be used to block network access for programs that are
dynamically linked to glibc, and usually works pretty good with programs
written in C.

## Disclaimer

USE AT YOUR OWN RISK. Most of this code was generated using GPT, and just
tweaked and formatted a bit, and it might be flawed in various ways. This is
also why there is no license attached, as it's simply hard to license code that
has been generated this way.

## Example

```
$ make
gcc -Wall -fPIC -o test test.c -lresolv
gcc -Wall -fPIC -shared -o netblock.so netblock.c -ldl
$ LD_PRELOAD=./netblock.so ./test
*** BLOCKED: connect(family=AF_INET, addr=192.0.2.1, port=80) ***
*** BLOCKED: connect(family=AF_INET6, addr=2001:db8::, port=80) ***
*** BLOCKED: connect(family=AF_UNIX, addr=/tmp/demo_socket) ***
*** BLOCKED: connect(family=AF_NETLINK) ***
*** BLOCKED: getaddrinfo(example.com) ***
*** BLOCKED: res_query(name=example.com) ***
*** BLOCKED: res_search(name=example.com) ***
*** BLOCKED: gethostbyname(name=example.com) ***
*** BLOCKED: gethostbyname2(name=example.com) ***
*** BLOCKED: getnameinfo(address=192.0.2.1) ***
*** BLOCKED: getnameinfo(address=2001:db8::) ***
*** BLOCKED: gethostbyaddr(address=192.0.2.1) ***
*** BLOCKED: gethostbyaddr(address=2001:db8::) ***
$ LD_PRELOAD=./netblock.so curl example.com
*** BLOCKED: getaddrinfo(example.com) ***
curl: (6) Could not resolve host: example.com
$ LD_PRELOAD=./netblock.so curl 192.0.2.1
*** BLOCKED: connect(family=AF_INET, addr=192.0.2.1, port=80) ***
curl: (7) Failed to connect to 192.0.2.1 port 80 after 0 ms: Couldn't connect to server
$ LD_PRELOAD=./netblock.so wget example.com
--2024-04-14 22:00:35--  http://example.com/
Resolving example.com (example.com)... *** BLOCKED: getaddrinfo(example.com) ***
failed: Non-recoverable failure in name resolution.
wget: unable to resolve host address 'example.com'
```
