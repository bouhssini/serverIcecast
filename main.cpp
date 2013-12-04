#include "General.h"

int sPORT;
vector<Clients> clients;
vector<Servers> servers;

int main(int argc, char *argv[])
{
    string txt = "abcdef";

    int p = txt.find("f");
    p = txt.find("g");
    p = txt.find("a");
    txt = txt.substr(p+1);


    sPORT = (argc > 1)?atoi(argv[1]):7000;
    int listener = Listen(sPORT);
    if (listener == -1)
    {
        perror("Port Deja Open");
        exit(-1);
    }
    while(1){
        usleep(100);
        if (AcceptServer(listener)) break;
        RecvClients();
        Removefail();
    }
    close(listener);
    return 0;
}
