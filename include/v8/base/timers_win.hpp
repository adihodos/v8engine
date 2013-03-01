//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

/**
 * \file timers_win.hpp
 */

#include <windows.h>
#include <cstdint>
#include <v8/v8.hpp>

namespace v8 { namespace base {

/**
 * A simple timer, built around the timeGetTime() api function.
 */
template<typename real_t>
class basic_timer {
private :
    DWORD   t0_;
    DWORD   t1_;

public :
    basic_timer() : t0_(0), t1_(1) {
        start();
    }

    void start() {
        t0_ = ::timeGetTime();
    }

    void stop() {
        t1_ = timeGetTime();
    }

    /**
     * Gets the elapsed time between the reset() and stop() calls.
     *
     * \return  The time interval, in milliseconds.
     */
    real_t get_delta_ms() const {
        return (real_t(t1_) - real_t(t0_));
    }

    real_t tick() {
        stop();
        real_t delta = get_delta_ms();
        t0_ = t1_;
        return delta;
    }
};

/**
 * High resolution timer, using QueryPerformanceCounter() api function.
 */
template<typename real_t>
class high_resolution_timer {
private :
    real_t	        perf_multiplier_;
    int64_t	        start_;
    int64_t	        end_;

public :
    high_resolution_timer() : perf_multiplier_(0), start_(0), end_(0) {
        int64_t perf_counts_per_second = 0;
        ::QueryPerformanceFrequency(
            reinterpret_cast<LARGE_INTEGER*>(&perf_counts_per_second));
        perf_counts_per_second /= 1000;
        perf_multiplier_ = 1 / (real_t) perf_counts_per_second;

        start();
    }

    void start() {
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_));
    }

    void stop() {
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&end_));
    }

    /**
     * Gets the elapsed time between the reset() and stop() calls.
     *
     * \return  The time interval, <b>in milliseconds</b>.
     */
    real_t get_delta_ms() const {
        return (real_t(end_) - real_t(start_)) * perf_multiplier_;
    }

    /**
     * \brief Returns the number of elapsed milliseconds from the previous call
     *  of start()/tick(). Also updates the start time to match end time,
     * so that time intervals are continuous.
     */
    real_t tick() {
        stop();
        real_t delta = get_delta_ms();
        start_ = end_;
        return delta;
    }
};

} // namespace base
} // namespace v8
