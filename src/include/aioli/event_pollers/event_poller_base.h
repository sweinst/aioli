#pragma once
#include <queue>
#include "../base.h"
#include "event_poller_types.h"

namespace aioli {
    /** Base class for event pollers */
    class EventPollerBase {
       public:
        virtual ~EventPollerBase() = default;

        /** Adds a timer that will expire at the specified deadline */
        TimerId add_timer(time_point deadline, coro_hdl handle) noexcept;

        /** Adds a timer that will expire after the specified duration */
        TimerId add_timer(chrono::milliseconds duration, coro_hdl handle) noexcept;

        /** Removes a timer by its unique id */
        bool remove_timer(const TimerId& timer_id) noexcept;

       protected:
        EventPollerBase(chrono::milliseconds max_poll_duration = chrono::milliseconds(100)) noexcept
            : max_poll_duration_{max_poll_duration}
            , max_poll_duration_ts_{get_timespec(max_poll_duration)} {}

        EventPollerBase(const EventPollerBase&) = delete;
        EventPollerBase& operator=(const EventPollerBase&) = delete;

        /** Gets the maximum duration to block while polling for events */
        const timespec get_event_poll_duration(time_point now) const noexcept;

        /** Processes all expired timers */
        void process_timers();

        /** Converts a time_point deadline to timespec format */
        const struct timespec get_timespec(time_point now, time_point deadline) const noexcept;
        /** Converts a duration in milliseconds to timespec format */
        static const struct timespec get_timespec(chrono::milliseconds duration) noexcept;

       private:
        /** the maximum duration to block while polling for events in  timespec format */
        const chrono::milliseconds max_poll_duration_;
        /** the maximum duration to block while polling for events in  timespec format */
        const struct timespec max_poll_duration_ts_;
        /** the timers are stored in a priority queue ordered by their deadlines */
        std::priority_queue<Timer> timers_;
        /** the next unique timer id to assign */
        id_t next_timer_id_ = 0;
        /** the last fired timer id */
        id_t last_fired_timer_ = INVALID_ID;
        /** the pending I/O events */
        std::vector<IOEvent> pending_events_;
        /** the ready I/O events after having processed the pending events with poll & co */
        std::vector<IOEvent> ready_events_;
    };

    inline const timespec EventPollerBase::get_event_poll_duration(time_point now) const noexcept {
        if (timers_.empty()) {
            return max_poll_duration_ts_;
        }
        return get_timespec(now, timers_.top().id_.deadline_);
    }

    inline TimerId EventPollerBase::add_timer(time_point deadline, coro_hdl handle) noexcept {
        Timer timer{
            .id_ = TimerId{.id_ = next_timer_id_++, .deadline_ = deadline},
            .handle_ = handle,
        };
        timers_.push(std::move(timer));
        return timer.id_;
    }

    inline TimerId EventPollerBase::add_timer(chrono::milliseconds duration, coro_hdl handle) noexcept {
        return add_timer(clock::now() + duration, handle);
    }

}  // namespace aio
