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

#ifndef ATOMIC_ULONG_H
#define ATOMIC_ULONG_H

namespace Threading
{
    class AtomicULong
    {
    public:
        AtomicULong() { Value = 0; }
        AtomicULong(unsigned long value) { Value = value; }

        /** 设置包含的值(无锁的线程安全操作) */
        unsigned long SetVal(unsigned long NewValue);

        /** 获取包含的值(非线程安全的) */
        unsigned long GetVal() { return Value; }

    private:
        /** 禁用拷贝构造函数 */
        AtomicULong(const AtomicULong &other) {}

        /** 禁用赋值操作符 */
        AtomicULong operator=(AtomicULong &other) { return *this; }

    protected:
        __declspec( align( 4 ) ) volatile unsigned long Value;
    };
}

#endif // ATOMIC_ULONG_H
