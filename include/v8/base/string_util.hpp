#pragma once

#include <v8/v8.hpp>

#if defined(V8_COMPILER_IS_MSVC)
#include <v8/base/string_util_msvc.hpp>
#else
#include <v8/base/string_util_gcc.hpp>
#endif 

