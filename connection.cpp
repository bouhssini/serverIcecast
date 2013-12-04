
#include "General.h"
/* sock_recoverable
**
** determines if the socket error is recoverable
** in terms of non blocking sockets
*/
int sock_recoverable(int error)
{
    switch (error)
    {
    case 0:
    case EAGAIN:
    case EINTR:
    case EINPROGRESS:
    case ERESTART:
        return 1;
    default:
        return 0;
    }
}

int sock_stalled (int error)
{
    switch (error)
    {
    case EAGAIN:
    case EINPROGRESS:
    case EALREADY:
    case ERESTART:
        return 1;
    default:
        return 0;
    }
}

static int sock_connect_pending (int error)
{
    return error == EINPROGRESS || error == EALREADY;
}

int sock_error(int sock)
{
    int val = SOCK_ERROR;
    socklen_t size = sizeof val;
    getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*) &val, &size);
    errno = val;
    return errno;
}

/* determines if the passed socket is still connected */
int sock_active (int sock)
{
    char c;
    int l;

    l = recv (sock, &c, 1, MSG_PEEK);
    if (l == 0)
        return 0; //close
    if (l > 0)
        return 1;

    if (l == SOCK_ERROR && !sock_recoverable (sock_error(sock)))
        return -1;//error
    else
        return 2;//no data

}


int sock_set_nodelay(int sock)
{
    int nodelay = 1;

    return setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&nodelay,
            sizeof(int));
}
int sock_set_nolinger(int sock)
{
    struct linger lin = { 0, 0 };
    return setsockopt(sock, SOL_SOCKET, SO_LINGER, (void *)&lin,
            sizeof(struct linger));
}
int sock_set_keepalive(int sock)
{
    int keepalive = 1;
    return setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive,
            sizeof(int));
}
int sock_set_noblocking(int sock, int yes)
{
    int rc = ioctl(sock, FIONBIO, &yes);
    return rc;
}

/* some win32 setups do not do TCP win scaling well, so allow an override */
void sock_set_send_buffer (int sock, int win_size)
{
    setsockopt (sock, SOL_SOCKET, SO_SNDBUF, (char *) &win_size, sizeof(win_size));
}
