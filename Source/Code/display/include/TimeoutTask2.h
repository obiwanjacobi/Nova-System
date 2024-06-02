/*
Arduino Template Library https://github.com/obiwanjacobi/atl
Written by Marc Jacobi
Copyright 2012-2015 All Rights Reserved

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __TIMEOUTTASK2_H__
#define __TIMEOUTTASK2_H__

#include <stdint.h>
#include "Scheduler.h"
#include "Task.h"

namespace ATL
{

    /** The TimeoutTask counts a timer down asynchronously (non-blocking).
     *  Calls the BaseT::OnTimeout1 when the Timeout1 has expired and then calls the BaseT::OnTimeout2 when the Timeout2 has expired.
     *  Call the Execute method repeatedly.
     *  \tparam BaseT is used as a base class and implements:
     *  `void OnTimeout()`
     *  `uint16_t getId()` (IdentifiableObject).
     *  \tparam SchedulerT is a Scheduler<> type used to keep track of timeouts.
     *  \tparam Timeout1 is specified in the same quantity as the SchedulerT is specified (Milli- or MicroSeconds).
     *  \tparam Timeout2 is specified in the same quantity as the SchedulerT is specified (Milli- or MicroSeconds).
     */
    template <class BaseT, typename SchedulerT, const uint32_t Timeout1, const uint32_t Timeout2>
    class TimeoutTask2 : public BaseT
    {
    public:
        /** Constructs the instance.
         */
        TimeoutTask2()
            : _task(0)
        {
        }

        /** Call this method repeatedly from the main loop.
         *  Each time the Timeout expires the BaseT::OnTimeout() method is called.
         */
        Task_Begin(Execute)
        {
            Task_YieldUntil(SchedulerT::Wait(BaseT::getId(), Timeout1));
            BaseT::OnTimeout1();
            Task_YieldUntil(SchedulerT::Wait(BaseT::getId(), Timeout2));
            BaseT::OnTimeout2();
        }
        Task_End;

        /** Returns the Timeout1 template parameter.
         */
        inline uint32_t getTimeout1() const
        {
            return Timeout1;
        }
        /** Returns the Timeout2 template parameter.
         */
        inline uint32_t getTimeout2() const
        {
            return Timeout2;
        }

    private:
        uint16_t _task; // Task state for the Task macros
    };

} // ATL

#endif //__TIMEOUTTASK2_H__