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

#ifndef ATOMIC_COUNTER_H
#define ATOMIC_COUNTER_H

namespace Threading
{
    class AtomicCounter : public AtomicULong
    {
    public:
        AtomicCounter() { Value = 0; }
        AtomicCounter(unsigned long InitialValue) { Value = InitialValue; }

        unsigned long operator++();
        unsigned long operator--();

    private:
        /** disabled */
        AtomicCounter(const AtomicCounter & other) {}

        AtomicCounter operator=(const AtomicCounter & other) { return *this; }
        
    };
}

#endif // ATOMIC_COUNTER_H
