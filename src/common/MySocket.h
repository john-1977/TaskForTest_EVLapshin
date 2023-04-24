#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include <sys/un.h>
#include <netinet/in.h>

#include <string>



class ClientSocket
{
    public:
    ClientSocket();
    ~ClientSocket();
    bool Connect (const std::string &addr, int port);
    bool Send (const unsigned char* buf, int len);
    int Recv(unsigned char *buf, int max_len);

    protected:
    int sockfd;
    struct sockaddr_in address;
};

class ServerClnt
{
public:    
    ServerClnt() = delete;
    ServerClnt(int client_sockfd);
    ~ServerClnt();
    bool Send ();
    int Recv ();
    bool SetData (const unsigned char *data, int len); //данные для записи в сокет
    std::pair<int, unsigned char*> GetData(); //получить буфр с данными и их длину

protected:
    const int _max_len{1024};
    int _clnt_sock;
    int _data_len;
    unsigned char* _buf;
};


class ServerSocket
{
public:
    ServerSocket();
    ~ServerSocket();
    bool Connect (int port);
    int Accept();
    
    
protected:
    int socksrv;
    struct sockaddr_in address;
};

