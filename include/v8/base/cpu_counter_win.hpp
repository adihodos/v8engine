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

#include <pdh.h>
#include <v8/v8.hpp>

namespace v8 { namespace base {

/*!
 * \class cpu_counter
 * \brief A class to get CPU usage information.
 * \todo { Does not offer option to specify which processor is queried, 
 *         always using processor 0 }
 */
class cpu_counter {
private :
    NO_CC_ASSIGN(cpu_counter);

    PDH_HQUERY      queryhandle_;
    PDH_HCOUNTER    counter_;
    bool            valid_;

public :
    cpu_counter() : valid_(false) {
        const wchar_t* const kPerfCntName = L"\\Processor(0)\\% Processor Time";
        valid_ = (::PdhOpenQueryW(nullptr, 0, &queryhandle_) == ERROR_SUCCESS)
                  && (::PdhAddCounterW(queryhandle_, kPerfCntName, 0, &counter_) 
                      == ERROR_SUCCESS);
    }

    ~cpu_counter() {
        if (valid_) {
            ::PdhRemoveCounter(counter_);
            ::PdhCloseQuery(queryhandle_);
        }
    }

    bool operator!() const {
        return !valid_;
    }

    double get_cpu_usage() const {
        double cpu_usage = 0.0;
        if (valid_) {
            if (::PdhCollectQueryData(queryhandle_) == ERROR_SUCCESS) {
                PDH_FMT_COUNTERVALUE cnt_val;
                if (::PdhGetFormattedCounterValue(counter_, PDH_FMT_DOUBLE, 
                                                  nullptr, &cnt_val) == ERROR_SUCCESS) {
                    cpu_usage = cnt_val.doubleValue;
                } 
            }
        }
        return cpu_usage;
    }
};

} // namespace base
} // namespace v8
