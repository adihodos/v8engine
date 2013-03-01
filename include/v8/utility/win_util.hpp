#pragma once

#include <crtdbg.h>

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>

namespace v8 { namespace utility {

void formatted_msg_box(
    const char* title,
    v8_uint_t buttons,
    const char* format,
    ...
    );

namespace win32 {

class scoped_mem_leak_checker {
public :
    scoped_mem_leak_checker(
        const v8_int_t report_flags = _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF
        ) {
        _CrtSetDbgFlag(report_flags);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
        _CrtMemCheckpoint(&initial_state_);
    }

    ~scoped_mem_leak_checker() {
        _CrtMemCheckpoint(&final_state_);
        _CrtMemState mem_diff;
        if (_CrtMemDifference(&mem_diff, &initial_state_, &final_state_)) {
            _CrtDumpMemoryLeaks();
        }
    }
private :
    _CrtMemState    initial_state_;
    _CrtMemState    final_state_;

private :
    NO_CC_ASSIGN(scoped_mem_leak_checker);
};

typedef v8::base::scoped_ptr
<
    wchar_t, 
    v8::base::default_array_storage
>                                           scoped_wide_string_t;

scoped_wide_string_t multibyte_string_to_wide_string(const char* mb_string);

}
}
}