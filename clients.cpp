#include "clients.h"

Clients::Clients()
{
    id = -1;
    server = -1;
    channel = -1;
    skey.clear();
    sWait = false;
    sCon = false;
    state = -1;
    sNet = 0;
    sPort = 0;
    RemotePort = 0;
    sPacket = 0;
    RemoteIP.clear();
    addressIP.clear();
    nickname.clear();
    sByte.clear();
}

Servers::Servers() : sock(-1), count(0),channl1(-1),channl2(-1)
{
    string head;
}

Servers::~Servers()
{

}

void Servers::set_mp3(int channel,string data)
{

}
