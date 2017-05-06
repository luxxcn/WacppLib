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

#ifndef AOTMIC_BOOLEAN_H
#define AOTMIC_BOOLEAN_H

namespace Threading
{
    class AtomicBoolean
    {
    public:
        AtomicBoolean() : Val(0) {}
        AtomicBoolean(bool val)
        {
            if(val)
                Val.SetVal(1);
            else
                Val.SetVal(0);
        }

        bool SetVal(bool val);

        bool GetVal()
        {
            unsigned long val = 0;

            val = Val.GetVal();

            return (val & 1);
        }

    private:
        /** disabled */
        AtomicBoolean(const AtomicBoolean & other) {}
        AtomicBoolean operator=(AtomicBoolean & ohter) { return *this; }

        AtomicULong Val;
    };
}

#endif // AOTMIC_BOOLEAN_H
