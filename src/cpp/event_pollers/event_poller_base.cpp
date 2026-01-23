#include <aioli/event_pollers/event_poller_base.h>

namespace aio {
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

    const struct timespec EventPollerBase::get_timespec(time_point now, time_point deadline) const noexcept {
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

}  // namespace aio
