/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * ListenSocket<T>: Creates a socket listener on specified address and port,
 *				  requires Update() to be called every loop.
 *
 */

#ifndef LISTEN_SOCKET_WIN32_H
#define LISTEN_SOCKET_WIN32_H

#ifdef CONFIG_USE_IOCP

#include "ThreadPool.h"

template<class T>
class SERVER_DECL ListenSocket : public ThreadBase
{
public:
    ListenSocket(const char * address, uint32 port)
    {
        m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
        SocketOps::ReuseAddr(m_socket);
        SocketOps::Blocking(m_socket);
        SocketOps::SetTimeout(m_socket, 60); // 60 秒超时

        m_address.sin_family = AF_INET;
        m_address.sin_port = ntohs((u_short)port);
        m_address.sin_addr.s_addr = htonl(INADDR_ANY);
        m_opened = false;

        if(strcmp(address, "0.0.0.0"))
        {
            struct hostent * hostname = gethostbyname(address);
            if(hostname)
                memcpy(&m_address.sin_addr.s_addr, hostname->h_addr_list[0], hostname->h_length);
        }

        // bind.. well attempt to.
        int ret = bind(m_socket, (const sockaddr*)&m_address, sizeof(m_address));
        if(ret != 0)
        {
            sLog.outError("Bind unsuccessful on port %u.", port);
            return;
        }

        ret = listen(m_socket, 5);// blacklog 连接请求队列的最大长度, SOMAXCONN 则由系统决定
        if(ret != 0) 
        {
            sLog.outError("Unable to listen on port %u.", port);
            return;
        }

        m_opened = true;
        len = sizeof(sockaddr_in);
        m_cp = sSocketMgr.GetCompletionPort();
    }

    ~ListenSocket()
    {
        Close();
    }

    bool run()
    {
        while(m_opened)
        {
            aSocket = WSAAccept(m_socket, (sockaddr*)&m_tempAddress, (socklen_t*)&len, NULL, NULL);
            if(aSocket == INVALID_SOCKET)
                continue; // shouldn't happen, we are blocking.

            socket = new T(aSocket);
            socket->SetCompletionPort(m_cp);
            socket->Accept(&m_tempAddress);
        }
        return false;
    }

    void Close()
    {
        // prevent a race condition here.
        bool mo = m_opened;
        m_opened = false;

        if(mo)
            SocketOps::CloseSocket(m_socket);
    }

    inline bool IsOpen() { return m_opened; }

private:
    SOCKET m_socket;
    struct sockaddr_in m_address;
    struct sockaddr_in m_tempAddress;
    bool m_opened;
    uint32 len;
    SOCKET aSocket;
    T * socket; 
    HANDLE m_cp;
};

#endif // CONFIG_USE_IOCP

#endif // LISTEN_SOCKET_WIN32_H
