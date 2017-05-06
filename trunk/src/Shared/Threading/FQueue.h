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

#ifndef FQUEUE_H
#define FQUEUE_H

template<class T>
class FQueue
{
public:
    FQueue()
    {
        first = last = NULL;
        m_size = 0;
    }

    uint32 GetSize()
    {
        lock.Acquire();
        ::uint32 ret = m_size;
        lock.Release();

        return ret;
    }

    void push(T & item)
    {
        lock.Acquire();

        h * p = new h;
        p->value = item;
        p->pNext = NULL;

        if(!m_size)
            first = p;
        else
            last->pNext = (h*)p;

        last = p;

        m_size++;

        lock.Release();
    }

    T pop()
    {
        lock.Acquire();

        if(!m_size || !first)
        {
            lock.Release();
            return NULL;
        }

        h * temp = first;
        
        if(--m_size)
            first = (h*)first->pNext;
        else
            first = last = NULL;

        lock.Release();

        T value = temp->value;
        delete temp;
        return value;
    }

private:
    struct h
    {
        T value;
        void * pNext;
    };

    h * first;
    h * last;
    volatile uint32 m_size;

    Mutex lock;
};

#endif // FQUEUE_H
