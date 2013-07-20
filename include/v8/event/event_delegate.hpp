#pragma once

#include <algorithm>
#include <vector>

#include <v8/fast_delegate/fast_delegate.hpp>

namespace v8 {

namespace detail {

template<typename delegate_type>
struct ehl_common_boilerplate {

    void operator+=(delegate_type new_delegate) {
        delegates_list_.push_back(new_delegate);
    }
    
    void operator-=(delegate_type existing_delegate) {
        delegates_list_.erase(
            std::remove(std::begin(delegates_list_), std::end(delegates_list_),
                        existing_delegate),
            std::end(delegates_list_));
    }
    
protected :
    ehl_common_boilerplate() {}

    ~ehl_common_boilerplate() {}
    
    std::vector<delegate_type>  delegates_list_;
};

} // namespace detail

///
/// \brief List for event handlers with no arguments.
template<typename ret_val = void>
class event_handler_list0
    : public detail::ehl_common_boilerplate<fastdelegate::FastDelegate0<ret_val>>  {

private :
    typedef detail::ehl_common_boilerplate
    <
        fastdelegate::FastDelegate0<ret_val>
    >                                                       base_class;

public :
    typedef fastdelegate::FastDelegate0<ret_val>            delegate_type;
    
    void call_delegates() {
        std::for_each(std::begin(base_class::delegates_list_),
                      std::end(base_class::delegates_list_),
                      [](delegate_type del) {
            del();
        });
    }
};

///
/// \brief List for event handlers with one argument.
template<typename arg0, typename ret_val = void>
class event_handler_list1
    : public detail::ehl_common_boilerplate<fastdelegate::FastDelegate1<arg0, ret_val>>  {

private :
    typedef detail::ehl_common_boilerplate
    <
        fastdelegate::FastDelegate1<arg0, ret_val>
    >                                                       base_class;
    
public :
    typedef fastdelegate::FastDelegate1<arg0, ret_val>      delegate_type;
    
    void call_delegates(const arg0& a0) {
        std::for_each(std::begin(base_class::delegates_list_),
                      std::end(base_class::delegates_list_),
                      [&a0](delegate_type del) {
            del(a0);
        });
    }
};

///
/// \brief List for event handlers with two arguments.
template<typename arg0, typename arg1, typename ret_val = void>
class event_handler_list2
    : public detail::ehl_common_boilerplate<fastdelegate::FastDelegate2<arg0, arg1, ret_val>>  {

private :
    typedef detail::ehl_common_boilerplate
    <
        fastdelegate::FastDelegate2<arg0, arg1, ret_val>
    >                                                           base_class;

public :
    typedef fastdelegate::FastDelegate2<arg0, arg1, ret_val>    delegate_type;
    
    void call_delegates(const arg0& a0, const arg1& a1) {
        std::for_each(std::begin(base_class::delegates_list_),
                      std::end(base_class::delegates_list_),
                      [&a0, &a1](delegate_type del) {
            del(a0, a1);
        });
    }
};

///
/// \brief List for event handlers with three arguments.
template<typename arg0, typename arg1, typename arg2, typename ret_val = void>
class event_handler_list3
    : public detail::ehl_common_boilerplate<fastdelegate::FastDelegate3<arg0, arg1, arg2, ret_val>>  {

private :
    typedef detail::ehl_common_boilerplate
    <
        fastdelegate::FastDelegate3<arg0, arg1, arg2, ret_val>
    >                                                               base_class;
    
public :
    typedef fastdelegate::FastDelegate3<arg0, arg1, arg2, ret_val>  delegate_type;
    
    void call_delegates(const arg0& a0, const arg1& a1, const arg2& a2) {
        std::for_each(std::begin(base_class::delegates_list_),
                      std::end(base_class::delegates_list_),
                      [&a0, &a1, &a2](delegate_type del) {
            del(a0, a1, a2);
        });
    }
};

} // namespace v8
