#include "MySocket.h"
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>


ClientSocket::ClientSocket() : sockfd{-1}
{
}

ClientSocket::~ClientSocket()
{
    if (sockfd > 0)
    {
        close (sockfd);
    }
}

bool ClientSocket::Connect(const std::string &addr, int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error socket creation" << std::endl;
        return false;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    
    struct hostent* hent = gethostbyname (addr.c_str());
    if (!hent)
    {
        std::cout << "Error: can't get address for " << addr << std::endl;
        return false;
    }
    char* ip_addr = inet_ntoa(*((struct in_addr*)hent->h_addr_list[0]));
    std::cout << "Found IP: " << ip_addr << std::endl;
    address.sin_addr.s_addr = inet_addr(ip_addr);

    int len = sizeof (address);
    int result = connect (sockfd, (struct sockaddr*)&address, len);
    if (result < 0 )
    {
        std::cerr << "Error connect: ";
        return false;
    }

    return true;
}

bool ClientSocket::Send(const unsigned char* buf, int len)
{
    int amount_sent = send (sockfd, buf, len, 0);
    if (amount_sent < len)
    {
        return false;
    }
    return true;
}

int ClientSocket::Recv(unsigned char *buf, int max_len)
{
    int readed = recvfrom (sockfd, buf, max_len, 0, NULL, NULL);
    return readed;
}



////////////////////////////////////////////////////////////////////////
ServerSocket::ServerSocket() : socksrv(-1)
{

}

ServerSocket::~ServerSocket()
{
    if (socksrv > 0)
    {
        close (socksrv);
    }
}

bool ServerSocket::Connect(int port)
{
    socksrv = socket(AF_INET, SOCK_STREAM, 0);
    if (socksrv < 0)
    {
        std::cerr << "Server error socket creation" << std::endl;
        return false;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    if (bind (socksrv, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        std::cout << "Server error bind." << std::endl;
        return false;
    }

    if (listen(socksrv, 1) < 0)
    {
        return false;
    }
    
    return true;
}

int ServerSocket::Accept()
{
    int sockclnt = accept(socksrv, NULL, NULL);
    if (sockclnt < 0)
    {
        std::cout << "Server error accept." << std::endl;
        return -1;
    }
    return sockclnt;
}


ServerClnt::ServerClnt(int client_sockfd) : _clnt_sock(client_sockfd), _data_len(0), _buf(nullptr)
{
    _buf = new unsigned char[_max_len];
}

ServerClnt::~ServerClnt()
{
    if (_buf)
    {
        delete [] _buf;
        _buf = nullptr;
    }
    close (_clnt_sock);
}

bool ServerClnt::Send()
{
    int amount_sent = send (_clnt_sock, _buf, _data_len, 0);
    if (amount_sent < _data_len)
    {
        return false;
    }
    return true;
}

int ServerClnt::Recv()
{
    int readed = recvfrom (_clnt_sock, _buf, _max_len, 0, NULL, NULL);
    if (readed >= 0)
    {
        _buf[readed] = 0;
        return readed;
    }
    return -1;
}

bool ServerClnt::SetData(const unsigned char *data, int len)
{
    if (len > _max_len) return false;
    memcpy (_buf, data, len);
    _data_len = len;
    return true;
}
