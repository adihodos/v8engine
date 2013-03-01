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

//!
//! \file timers_posix.hpp

#include <sys/time.h>
#include <time.h>
#include <string.h>

#include <cassert>

#include <v8/v8.hpp>

namespace v8 { namespace base {

//!
//! \brief Timer class, with microsecond resolution.
template<typename real_t>
class basic_timer {
public :

    //! \name Typedefs.
    //! @{

    typedef basic_timer<real_t>                         class_type;

    //! @}

public :

    //! \name Constructors.
    //! @{

    //! \brief Default constructor. Object is not initialized, start() must be
    //!  called before using any other methods.
    basic_timer() {}

    //! @}

public :

    //! \name Timing routines.
    //! @{

    //!
    //! \brief Starts the timer.
    void start() {
        gettimeofday(&t0_, nullptr);
        t1_ = t0_;
    }

    //!
    //! \brief Stops the timer.
    void stop() {
        gettimeofday(&t1_, nullptr);
    }

    //!
    //! \brief Computes the elapsed time interval, between start() and stop()
    //! calls.
    //! \return Elapsed time, in milliseconds.
    real_t get_delta_ms() const {
        timeval diff_tm;
        sub_timeval(&t0_, &t1_, &diff_tm);
        return diff_tm.tv_sec * real_t(1000.0f)
               + diff_tm.tv_usec * real_t(1.0e-3f);
    }

    //!
    //! \brief Computes elapsed time between continuos time intervals.
    //! \return Elapsed time, since the last call of tick() (if any) and the
    //! timestamp the the moment of call. Expressed in milliseconds.
    real_t tick() {
        stop();
        const real_t delta_ms = get_delta_ms();
        t0_ = t1_;
        return delta_ms;
    }

    //! @}

private :

    //! \name Internal helper routines.
    //! @{

    //!
    //! \brief Substracts two timeval timestamps.
    //! \param[in] start Pointer to the start timestamp.
    //! \param[in] end Pointer to the end timestamp.
    //! \param[in,out] result Stores result of (end - start).
    void sub_timeval(
        const struct timeval* start,
        const struct timeval* end,
        struct timeval* result
        ) const {

        assert(end->tv_sec >= start->tv_sec && "Mismatched time intervals");

        if (end->tv_usec < start->tv_usec) {
            result->tv_sec = end->tv_sec - start->tv_sec - 1;
            result->tv_usec = end->tv_usec + (1E6L - start->tv_usec);
        } else {
            result->tv_sec = end->tv_sec - start->tv_sec;
            result->tv_usec = end->tv_usec - start->tv_usec;
        }
    }

    real_t timeval_to_micros(
        const timeval* stamp
        ) const {
        return static_cast<real_t>(stamp->tv_sec * 1E6 + stamp->tv_usec);
    }

    //! @}

private :

    //! \name Data members.
    //! @{

    timeval     t0_;    //!< Start timestamp.

    timeval     t1_;    //!< End timestamp.

    //! @}
};

//!
//! \brief High resolution time class for POSIX like operating systems.
template<typename real_t>
class high_resolution_timer {
public :

    //! \name Typedefs.
    //! @{

    typedef high_resolution_timer<real_t>                         class_type;

    //! @}

public :

    //! \name Constructors.
    //! @{

    //! \brief Default constructor. Object is not initialized, start() must be
    //!  called before using any other methods.
    high_resolution_timer() {
        struct timespec clock_res;
        clock_getres(CLOCK_MONOTONIC, &clock_res);
        clock_res_ = real_t(1.0f) / timespec_to_nanos(&clock_res);
    }

    //! @}

public :

    //! \name Timing routines.
    //! @{

    //!
    //! \brief Starts the timer.
    void start() {
        clock_gettime(CLOCK_MONOTONIC, &t0_);
        t1_ = t0_;
    }

    //!
    //! \brief Stops the timer.
    void stop() {
        clock_gettime(CLOCK_MONOTONIC, &t1_);
    }

    //!
    //! \brief Computes the elapsed time interval, between start() and stop()
    //! calls.
    //! \return Elapsed time, in milliseconds.
    real_t get_delta_ms() const {
        struct timespec delta_timespec;
        sub_timespec(&t0_, &t1_, &delta_timespec);
        real_t delta_nanos = timespec_to_nanos(&delta_timespec) * clock_res_;
        return delta_nanos * real_t(1.0e-6f);
    }

    //!
    //! \brief Computes elapsed time between continuos time intervals.
    //! \return Elapsed time, since the last call of tick() (if any) and the
    //! timestamp the the moment of call. Expressed in milliseconds.
    real_t tick() {
        stop();
        const real_t delta_ms = get_delta_ms();
        t0_ = t1_;
        return delta_ms;
    }

    //! @}

private :

    //! \name Data members.
    //! @{

    real_t                  clock_res_; //!< Clock resolution, in nanoseconds.

    struct timespec         t0_;        //!< Start time stamp.

    struct timespec         t1_;        //!< End time stamp.

    //! @}

private :

    //! \name Helper routines.
    //! @{

    //!
    //! \brief Substracts two timespec timestamps.
    //! \param[in] start Pointer to the start timestamp.
    //! \param[in] end Pointer to the end timestamp.
    //! \param[in,out] result Stores result of (end - start).
    void sub_timespec(
        const struct timespec* start,
        const struct timespec* end,
        struct timespec* result
        ) const {

        assert(end->tv_sec >= start->tv_sec && "Mismatched time intervals");

        if (end->tv_nsec < start->tv_nsec) {
            result->tv_sec = end->tv_sec - start->tv_sec - 1;
            result->tv_nsec = end->tv_nsec + (1E9L - start->tv_nsec);
        } else {
            result->tv_sec = end->tv_sec - start->tv_sec;
            result->tv_nsec = end->tv_nsec - start->tv_nsec;
        }
    }

    //!
    //! \brief Converts a timespec value to a value expressed in seconds.
    real_t timespec_to_seconds(const struct timespec* tspec) const {
        real_t tm_real = static_cast<real_t>(tspec->tv_sec);
        tm_real += static_cast<real_t>(tspec->tv_nsec) * real_t(1.0e-9f);
        return tm_real;
    }

    //!
    //! \brief Converts a timespec value to a value expressed in nanoseconds.
    real_t timespec_to_nanos(const struct timespec* tspec) const {
        return tspec->tv_sec * real_t(1.0e9f) + tspec->tv_nsec;
    }

    //! @}

private :

    //! \name Disabled operations.
    //! @{

    //NO_CC_ASSIGN(high_resolution_timer);

    //! @}
};

} // namespace base
} // namespace v8
