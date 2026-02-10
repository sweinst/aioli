#include <aioli/event_pollers/event_poller_base.h>

namespace aioli {
    void EventPollerBase::process_timers() {
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
                last_fired_timer_ = timer.id_.id_;
                timer.handle_.resume();
            }
            timers_.pop();
        }
    }

    const struct timespec EventPollerBase::get_timespec(chrono::milliseconds duration) noexcept {
        auto d_sec = chrono::duration_cast<chrono::seconds>(duration);
        auto d_nsec = chrono::duration_cast<chrono::nanoseconds>(duration - d_sec);
        return timespec{
            .tv_sec = static_cast<decltype(std::declval<timespec>().tv_sec)>(d_sec.count()),
            .tv_nsec = static_cast<decltype(std::declval<timespec>().tv_nsec)>(d_nsec.count()),
        };
    }

    const struct timespec
    EventPollerBase::get_timespec(time_point now, time_point deadline) const noexcept {
        if (deadline <= now) {
            return timespec{
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

    bool EventPollerBase::remove_timer(const TimerId& timer_id) noexcept {
        // already fired?
        if (timers_.empty() || timer_id.deadline_ < timers_.top().id_.deadline_ ||
            timer_id.id_ <= last_fired_timer_) {
            return false;
        }

        // we push a timer on the priority queue with no handle, it will be placed just before
        // the timer we want to cancel so the sequence of (timer_id, no handle) (timer_id.
        // handle) is considered as a timer cancellation
        Timer timer{
            .id_ = timer_id,
            .handle_ = nullptr,
        };
        timers_.push(std::move(timer));
        return true;
    }
}  // namespace aio
