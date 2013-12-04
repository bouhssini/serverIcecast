#ifndef CLIENTS_H
#define CLIENTS_H

#include <iostream>
#include <string>

using namespace std;

class Clients{
public:
    int id;
    bool sWait;    //خاص بالبروتوكول
    bool sCon;     //خاص بالبروتوكول
    int state;
    int sNet;
    int sPort;
    int RemotePort;
    int sPacket;

    string mount;
    string head;
    string skey;
    string RemoteIP;
    string addressIP;
    string nickname;
    string sByte;

    Clients();
};

class Servers{
public:
    Servers();
    ~Servers();
    int sock;
    int count;
    int channl1;
    int channl2;

    void set_mp3(int channel,string data);
    string head;

};

#endif // CLIENTS_H
