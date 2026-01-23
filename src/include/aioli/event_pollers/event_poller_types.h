#pragma once

namespace aio {
    /** Concept for event poller types */
    template <class EventPoller>
    concept EventPoller_t = requires(EventPoller ep) {
        { ep.poll_events() };
        { ep.wake_up_ready_tasks() };
        { ep.stop() };
    };
}  // namespace aio
