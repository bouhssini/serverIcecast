#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/ioctl.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string>
#include <time.h>
#include <iconv.h>
#include <signal.h>

#include <errno.h>
#include <err.h>


#include <netinet/tcp.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/fcntl.h>


using namespace std;

#include "base64.h"
#include "md5.h"
#include "clients.h"

#define SOCK_ERROR (int)-1
#define  WIN_SIZE (int)8192

//variable
extern vector<Clients> clients;
extern vector<Servers> servers;

//function tools
extern short cts(unsigned char * ch);
extern int cti(unsigned char * ch);
extern string stc(short i);
extern string itc(int i);
extern string convertInt(int value);
extern string itostr(int value);
extern int strtoi(string s);

extern string Replace(string txt,string str,string chstr);
extern string Trim(string txt);
extern string String(int Ln,char c);
extern string ICONV(string txt,const char fromcode[],const char tocode[]);
extern vector<string> Split(string txt,string str);
extern string UCASE(string txt);
extern string LCASE(string txt);

//connection
extern  int sock_recoverable(int error);
extern  int sock_stalled (int error);
static  int sock_connect_pending (int error);
extern  int sock_error(int sock);
extern  int sock_active (int sock);
extern  int sock_set_nodelay(int sock);
extern  int sock_set_nolinger(int sock);
extern  int sock_set_keepalive(int sock);
extern  int sock_set_noblocking(int sock, int yes);
extern void sock_set_send_buffer (int sock, int win_size);

//Server
extern  int Listen(int port);
extern  int AcceptServer(int srv);
extern void sock_recv(int index);
extern void RecvClients();
extern void Removefail();
extern void test_packet(int index);


#endif // GENERAL_H
