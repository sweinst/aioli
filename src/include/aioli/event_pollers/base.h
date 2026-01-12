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
            timers_.push(std::move(timer));
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
            timers_.push(std::move(timer));
            return true;
        }

    protected:
        EventPollerBase(chrono::milliseconds max_poll_duration = chrono::milliseconds(100)) noexcept
        : max_poll_duration_{max_poll_duration}, max_poll_duration_ts_{get_timespec(max_poll_duration)} {
        }

        EventPollerBase(const EventPollerBase&) = delete;
        EventPollerBase& operator=(const EventPollerBase&) = delete;

        /** Gets the maximum duration to block while polling for events */
        const timespec get_event_poll_duration(time_point now) const noexcept {
            if (timers_.empty()) {
                return max_poll_duration_ts_;
            }
            return get_timespec(now, timers_.top().id_.deadline_);
        }

        /** Processes all expired timers */
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

        /** Converts a time_point deadline to timespec format */
        const struct timespec get_timespec(time_point now, time_point deadline) const noexcept {
            if (deadline <= now) {
                return timespec {
                    .tv_sec = 0,
                    .tv_nsec = 0,
                };
            }
            auto duration = (deadline - now);
            if (duration > max_poll_duration_) {
                duration = max_poll_duration_;
            }
            return get_timespec(chrono::duration_cast<chrono::milliseconds>(duration));
        }

        /** Converts a duration in milliseconds to timespec format */
        const struct timespec get_timespec(chrono::milliseconds duration) const noexcept {
            auto d_sec = chrono::duration_cast<chrono::seconds>(duration);
            auto d_nsec = chrono::duration_cast<chrono::nanoseconds>(duration - d_sec);
            return timespec {
                .tv_sec = static_cast<decltype(std::declval<timespec>().tv_sec)>(d_sec.count()),
                .tv_nsec = static_cast<decltype(std::declval<timespec>().tv_nsec)>(d_nsec.count()),
            };
        }

    private:
        // the maximum duration to block while polling for events in  timespec format
        const chrono::milliseconds max_poll_duration_;
        // the maximum duration to block while polling for events in  timespec format
        const struct timespec max_poll_duration_ts_;
        // the timers are stored in a priority queue ordered by their deadlines
        std::priority_queue<Timer> timers_;
        // the next unique timer id to assign
        id_t next_timer_id_ = 0;
        // the last fired timer id
        id_t last_fired_timer = static_cast<id_t>(-1);
    };
} // namespace aio
