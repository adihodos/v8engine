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

/*!
 * \file auto_lock.hpp
 */

#include <v8/v8.hpp>

namespace v8 { namespace base {
  
/*!
 * \defgroup synchronization	
 *  Classes and utilities for synchronization in multithreaded code.
 */

/*! \addtogroup synchronization
 * @{
 */

/**
 * \brief Helper class to automate acquiring and releasing a lock. It will
 *      acquire the lock in the constructor and release it in the
 *      destructor. It is designed so that it can be used with a class
 *      that abstracts a locking primitive, provided that the class has
 *      two functions called acquire() and release() in the public part of
 *      its interface.
 */
template<typename LockT>
class auto_lock {
private :
    NO_CC_ASSIGN(auto_lock);
    
    /*!< Reference to an existing locking class object */
    LockT&  lock_;

public :
    /**
     * \brief auto_lock Constructs an object of this class and acquire the lock.
     * \param lock Reference to a "lock class object". Its public interface must
     *      implement two functions called acquire() and release().
     * \see scoped_lock class.
     */
    auto_lock(LockT& lock) : lock_(lock) {
        lock_.acquire();
    }

    /**
     * \brief The destructor releases the lock when this object expires.
     */
    ~auto_lock() {
        lock_.release();
    }
};

/*! @} */

} // namespace base
} // namespace v8
