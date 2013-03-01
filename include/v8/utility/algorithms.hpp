#pragma once

#include <iterator>

namespace v8 { namespace utility {

template<typename Container_Type, typename Comparator_Type>
typename Container_Type::iterator 
binary_search_container(
    Container_Type& cont,
    Comparator_Type cmp
    ) {
    size_t low = 0;
    size_t high = cont.size();

    while (low < high) {
        const size_t middle = (low + high) / 2;
        const int cmp_result = cmp(cont[middle]);

        if (cmp_result < 0) {
            low = middle + 1;
        } else if (cmp_result > 0) {
            high = middle;
        } else {
            return std::begin(cont) + middle;
        }
    }

    return std::end(cont);
}

template<typename InputIter, typename OutputIter, 
         typename Predicate, typename Func> 
inline OutputIter transform_if(
    InputIter in_first,
    InputIter in_last,
    OutputIter dest,
    Predicate pred__,
    Func fn__) {

    while (in_first != in_last) {
        if (pred__(*in_first))
            *dest++ = fn__(*in_first);
        ++in_first;
    }

    return dest;
}

} // namespace utility
} // namespace v8
