#include <cassert>
#include "v8/base/resource_policies/ref_link_base.hpp"

v8_bool_t v8::base::internal::ref_linked_base::release() {
    if (!m_next) {

        //
        // Nothing to to, already released.
        assert(!m_prev);
        return false;

    } else if (m_next == this) {
        assert(m_prev = this);

        //
        // Set to null to avoid double deletion.
        m_prev = m_next = nullptr;
        return true;
    }

    //
    // Unlink node from list.
    m_prev->m_next = m_next;
    m_next->m_prev = m_prev;

    return false;
}

void v8::base::internal::ref_linked_base::swap(ref_linked_base &rhs) {
    if (m_next == this) {
        //
        // Node only references itself.
        assert(m_prev == this);

        if (rhs.m_next == &rhs) {
            //
            // Other node only references itself, nothing to do.
            assert(rhs.m_prev == &rhs);
            return;
        }

        m_prev = rhs.m_prev;
        m_next = rhs.m_next;

        m_prev->m_next = this;
        m_next->m_prev = this;

        rhs.m_next = &rhs;
        rhs.m_prev = &rhs;

        return;
    }

    if (rhs.m_next == &rhs) {
        rhs.swap(*this);
        return;
    }

    if (m_next == &rhs) {
        //
        // Only two pointers, no swap needed.
        if (m_prev == &rhs) {
            return;
        }

        std::swap(m_prev, m_next);
        std::swap(rhs.m_prev, rhs.m_next);
        std::swap(rhs.m_prev, m_next);
        std::swap(rhs.m_prev->m_next, m_next->m_prev);

    } else if (m_prev == &rhs) {
        //
        // Two pointer cycle.
        if (m_next == &rhs) {
            return;
        }

        std::swap(m_prev, m_next);
        std::swap(rhs.m_next, rhs.m_prev);
        std::swap(rhs.m_next, m_prev);
        std::swap(rhs.m_next->m_prev, m_prev->m_next);
    } else {
        std::swap(m_prev, rhs.m_prev);
        std::swap(m_next, rhs.m_next);

        std::swap(m_prev->m_next, rhs.m_prev->m_next);
        std::swap(m_next->m_prev, rhs.m_next->m_prev);
    }
}
