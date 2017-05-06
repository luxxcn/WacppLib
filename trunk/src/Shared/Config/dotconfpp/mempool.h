/*
 * Copyright (C) 2011 <http://www.wacpp.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ASYNC_DNS_MEMPOOL_H
#define ASYNC_DNS_MEMPOOL_H

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

class AsyncDNSMemPool
{
    private:

        struct PoolChunk
        {
            void* pool;
            size_t pos;
            size_t size;
            
            PoolChunk(size_t _size);
            ~PoolChunk();
        };

        PoolChunk** chunks;
        size_t chunksCount;
        size_t defaultSize;

        size_t poolUsage;
        size_t poolUsageCounter;

        void addNewChunk(size_t size);

    public:

        AsyncDNSMemPool(size_t _defaultSize = 4096);
        virtual ~AsyncDNSMemPool();

        bool initialize();
        void Free();
        void* Alloc(size_t size);
        void* Calloc(size_t size);
        char* Strdup(const char *str);
};

#endif
