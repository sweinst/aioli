#pragma once
#include "../base.h"
#include "types.h"
#include <queue>

namespace aio {
    /** Base class for event pollers */
    class EventPollerBase {
    public:
        virtual ~EventPollerBase() = default;

        /** Adds a timer that will expire at the specified deadline */
        TimerId add_timer(time_point deadline, coro_handle handle) noexcept {
            Timer timer {
                .id_ = TimerId {
                    .id_ = next_timer_id_++,
                    .deadline_ = deadline
                },
                .handle_ = handle, 
            };
            timers_.push(timer);
            return timer.id_;
        }

        /** Adds a timer that will expire after the specified duration */
        TimerId add_timer(chrono::milliseconds duration, coro_handle handle) noexcept {
            return add_timer(clock::now() + duration, handle);
        }

        /** Removes a timer by its unique id */
        bool remove_timer(const TimerId& timer_id) noexcept {
            // already fired?
            if (timers_.empty()|| timer_id.deadline_ < timers_.top().id_.deadline_ || timer_id.id_ <= last_fired_timer) {
                return false;
            }

            // we push a timer on the priority queue with no handle, it will be placed just before the timer we want to cancel
            // so the sequence of (timer_id, no handle) (timer_id. handle) is considered as a timer cancellation
            Timer timer {
                .id_ = timer_id,
                .handle_ = nullptr,
            };
            timers_.push(timer);
            return true;
        }

    protected:
        EventPollerBase(chrono::milliseconds max_poll_duration = chrono::milliseconds(100))
            : max_poll_duration_(max_poll_duration) {
        }

        EventPollerBase(const EventPollerBase&) = delete;
        EventPollerBase& operator=(const EventPollerBase&) = delete;

        void process_timers() noexcept {
            time_point now = clock::now();
            id_t prev_id = 0;
            bool start = true;
            while (!timers_.empty()) {
                const Timer& timer = timers_.top();
                if (timer.id_.deadline_ > now) {
                    break;
                }
                // skip cancelled timers
                if ((start || timer.id_.id_ != prev_id) && timer.handle_ != nullptr) {
                    start = false;
                    prev_id = timer.id_.id_;
                    last_fired_timer = timer.id_.id_;
                    timer.handle_.resume();
                }
                timers_.pop();
            }
        }

    private:
        // the maximum duration to block while polling for events
        chrono::milliseconds max_poll_duration_;
        // the timers are stored in a priority queue ordered by their deadlines
        std::priority_queue<Timer> timers_;
        // the next unique timer id to assign
        id_t next_timer_id_ = 0;
        // the last fired timer id
        id_t last_fired_timer = static_cast<id_t>(-1);
    };
} // namespace aio
