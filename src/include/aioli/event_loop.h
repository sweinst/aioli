# pragma once

#include "base.h"
#include "event_pollers/event_poller_types.h"

namespace aioli {
    /** Event loop class which is run periodically for polling events */
    template <EventPoller_t EventPoller>
    class event_loop {
    public:
        void go() noexcept {
            running_ = true;
            while (running_) {
                poller_.poll_events();
                poller_.wake_up_ready_tasks();
            }
        }

        void stop() noexcept {
            running_ = false;
            poller_.stop();
        }

        EventPoller& get_poller() noexcept {
            return poller_;
        }

    private:
        EventPoller poller_;
        bool running_;
    };
} // namespace aio
