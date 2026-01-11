#pragma once

#include "base.h"

namespace aio {
    /** Timer identifier structure  */
    struct TimerId {
        /** unique timer id */
        id_t id_;
        /** when the timer expires */
        time_point deadline_;
        
        std::strong_ordering operator<=>(const TimerId& other) const noexcept {
            std::strong_ordering cmp_dl = deadline_ <=> other.deadline_;
            if (cmp_dl != std::strong_ordering::equal) {
                return cmp_dl;
            }
            return id_ <=> other.id_;
        }
    };

    /** Timer class for coroutine-based delays */
    struct Timer {
        /** unique timer id */
        TimerId id_;
        /** coroutine handle to resume */
        coro_handle handle_;

        std::strong_ordering operator<=>(const Timer& other) const noexcept {
            std::strong_ordering cmp_id = id_ <=> other.id_;
            if (cmp_id != std::strong_ordering::equal) {
                return cmp_id;
            }
            // Timers without coroutine handles are considered "less" than those with handles
            // See the code for canceling a timer
            return ((bool)handle_) <=> (bool)other.handle_;
        }
    };
} // namespace aio
