#include "General.h"

void cliens_close(int index);
void sock_remove(int index);

int Listen(int port)
{
    struct sockaddr_in myaddr;     // server address
    int listener;                  // listening socket descriptor
    int yes=1;                     // for setsockopt() SO_REUSEADDR, below

    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }
    // lose the pesky "address already in use" error message


    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const void *)&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        return -1;
    }

/*********************************************************************/
/*   Set socket to be nonblocking. All of the sockets for            */
/*   the incoming connections will also be nonblocking since         */
/*   they will inherit that state from the listening socket.         */
/*********************************************************************/

     sock_set_noblocking(listener,yes);

/********************************************************************/


    // bind
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(port);
    memset(&(myaddr.sin_zero),'\0', 8);
    if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
        perror("bind");
        return -1;
    }
    // listen
    if (listen(listener, 0) == -1) {
        perror("listen");
        return -1;
    }
    /* some win32 setups do not do TCP win scaling well, so allow an override */

    sock_set_send_buffer(listener,WIN_SIZE);
    return listener;
}
int AcceptServer(int srv)
{
    struct sockaddr_in remoteaddr; // client address
    int newfd;                     // newly accept()ed socket descriptor
    int addrlen;
    int yes=1;

    addrlen = sizeof(remoteaddr);


    if ((newfd = accept(srv, (struct sockaddr *)&remoteaddr,(socklen_t *)&addrlen)) != -1)
    {
        sock_set_nolinger(newfd);
        sock_set_nodelay(newfd);
        sock_set_keepalive(newfd);
        sock_set_send_buffer(newfd,WIN_SIZE);
        int rc = sock_set_noblocking(newfd,yes);
        Clients cls;
        cls.id = newfd;
        /*cls.sPoll.fd = newfd;
        cls.sPoll.events = POLLIN;
        cls.sPoll.revents = 0;*/
        cls.addressIP = inet_ntoa(remoteaddr.sin_addr);
        cls.sPort = ntohs(remoteaddr.sin_port);
        cls.skey = cls.addressIP+":"+ itostr(cls.sPort);
        cls.sNet = AF_INET;
        cls.state = 7;
        clients.push_back(cls);
    }else{
        int ret = 0;
        ret = sock_error(srv);
        if (ret) return -1;
    }
    return 0;
}
void RecvClients()
{
    for(int i = 0; i < clients.size(); i++)
    {
        if (clients[i].state == 7 && clients[i].id != -1){
            int ret = 0;
            ret = sock_active(clients[i].id);
            if (ret ==  1) sock_recv(i);     /*  data                  */
            if (ret == -1 || ret ==  0) cliens_close(i); /*  -1 = error 0 = close  */
            if (ret ==  2) continue;                     /*  nodata                */
        }
    }
}
void Removefail()
{
    aa:
    for(int i = 0; i < clients.size(); i++)
    {
        if (clients[i].state != 7 && clients[i].id == -1){
           sock_remove(i);
           goto aa;
        }
    }
}
void sock_recv(int index)
{
    char buffer[1024];
    int nbytes = 0;
    int sock = clients[index].id;
    string txt;
    txt.clear();
    aa:
    memset(&buffer,'\0',sizeof(buffer));
    if ((nbytes = recv(sock, buffer, sizeof(buffer), MSG_NOSIGNAL)) <= 0) {
       if (nbytes != 0) {
           perror("recv");
       }
    }
    clients[index].sByte.append(buffer,nbytes);
    test_packet(index);
}
void cliens_close(int index)
{
    close(clients[index].id); // bye!
    clients[index].state = 0;
    clients[index].id = -1;
}
void sock_remove(int index)
{
    clients.erase(clients.begin()+index);
}
void test_packet(int index)
{
    string txt;
    if (clients[index].sCon == false){
        int pos = clients[index].sByte.find("\n\n");
        if (pos != -1)
        {
            clients[index].head  = clients[index].sByte.substr(0,pos);
            clients[index].sByte = clients[index].sByte.substr(pos+2);
            clients[index].sCon  = true;

            if (get_server(index)){
                txt = "HTTP/1.0 200 OK\r\n\r\n";
                send(clients[index].id,txt.c_str(),txt.size(),0);
            }else{
                txt = "HTTP/1.0 403 Mountpoint in use\r\nContent-Type: text/html\r\n\r\n";
                send(clients[index].id,txt.c_str(),txt.size(),0);
            }
        }
    }
}

bool get_server(int index)
{
    Servers sr;
    Clients cl;
    bool fnd = false;
    int cnt = 0;

    for (int i= 0;i < servers.size(); i++)
    {
        if (servers[i].head == clients[index].head){
            if (servers[i].count < 3){
                clients[index].server = i;
                fnd = true;
                if (servers[i].channl1 == -1){
                    clients[index].channel = 1;
                    servers[i].channl1 = clients[index].id;
                }else{
                    clients[index].channel = 2;
                    servers[i].channl2 = clients[index].id;
                }
                return true;
            }else{
                fnd = false;
                cnt = 3;
                return false;
            }
            break;
        }
    }
    if ( cnt == 0 && fnd == false){
        sr.channl1 = clients[index].id;
        sr.head = clients[index].head;
        servers.push_back(sr);
    }
    return true;
}
string OpenURL()
{
    int sockd;
    int yes = 1;

        struct sockaddr_in serv_name;
        int status;
        /* create a socket */
        sockd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockd == -1)
        {
            perror("Socket creation");
            exit(1);
        }

        /* server address */
        serv_name.sin_family = AF_INET;
        inet_aton("localhost", &serv_name.sin_addr);
        serv_name.sin_port = htons(8000);


        sock_set_nolinger(sockd);
        sock_set_nodelay(sockd);
        sock_set_keepalive(sockd);
        sock_set_send_buffer(sockd,WIN_SIZE);
        int rc = sock_set_noblocking(sockd,yes);
        /* connect to the server */
        status = connect(sockd, (struct sockaddr*)&serv_name, sizeof(serv_name));
        if (status == -1)
        {
            perror("Connection error");
            exit(1);
        }


        close(sockd);
}
