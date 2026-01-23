#pragma once
#include "event_poller_base.h"

namespace aio {

    class SelectEventPoller : public EventPollerBase {
    public:
        SelectEventPoller();
        ~SelectEventPoller();

        void poll_events();
        void wake_up_ready_tasks();
        void stop();
    };

    static_assert(EventPoller_t<SelectEventPoller>);
} // namespace aio
