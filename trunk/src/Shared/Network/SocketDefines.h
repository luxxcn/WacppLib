/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketDefines.h - Any platform-specific defines/includes go here.
 *
 */

// 暂仅保留了win32平台相关

#ifndef SOCKET_DEFINES_H
#define SOCKET_DEFINES_H

#include "Common.h"
using namespace std;

//#ifdef WIN32
#define CONFIG_USE_IOCP

enum SocketIOEvent
{
    SOCKET_IO_EVENT_READ_COMPLETE = 0,
    SOCKET_IO_EVENT_WRITE_END     = 1,
    SOCKET_IO_THREAD_SHUTDOWN     = 2,
    NUM_SOCKET_IO_EVENTS          = 3,
};

class OverlappedStruct
{
public:
    OVERLAPPED m_overlap;
    SocketIOEvent m_event;
    volatile long m_inUse;

    OverlappedStruct(SocketIOEvent ev) : m_event(ev)
    {
        memset(&m_overlap, 0, sizeof(OVERLAPPED));
        m_inUse = 0;
    }

    OverlappedStruct()
    {
        memset(&m_overlap, 0, sizeof(OVERLAPPED));
        m_inUse = 0;
    }

    __forceinline void Reset(SocketIOEvent ev)
    {
        memset(&m_overlap, 0, sizeof(OVERLAPPED));
        m_event = ev;
    }

    void Mark()
    {
        unsigned long val = 0;
        val = InterlockedExchange(&m_inUse, 1);
        if(val != 0)
            sLog.outError("!!!! Network: Detected double use of read/write event! Previous event was %u.", m_event);
    }

    void Unmark()
    {
        InterlockedExchange(&m_inUse, 0);
    }
};

#endif // SOCKET_DEFINES_H
