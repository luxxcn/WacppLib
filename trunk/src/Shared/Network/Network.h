/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * Socket implementable class.
 *
 */

#ifndef NETWORK_H
#define NETWORK_H

#include "Log.h"
#include "CircularBuffer.h"
#include "SocketDefines.h"
#include "SocketOps.h"
#include "Socket.h"

#ifdef CONFIG_USE_IOCP
#include "SocketMgrWin32.h"
#include "ListenSocketWin32.h"
#endif


#endif // NETWORK_H
