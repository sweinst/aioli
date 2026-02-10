# pragma once
#include "base.h"

namespace aioli {
    /** I/O events */
    struct IOEvent {
        /** The event types defined as bits */
        enum Type {
            INVALID = 0x00,
            READ = 0x01,
            WRITE =0x02,
            // Stream socket peer closed connection, or shut down writing half of connection.
            // see poll(2) for details
            RHUP = 0x04, 
        };
        /** Which event types are requested */
        unsigned type_ = IOEvent::INVALID;
        /** Associated socket file descriptor */
        SOCKET fd_ = INVALID_SOCKET;
        /** Associated deadline timer ID if any */
        TimerId deadline_;
    };
} // namespace aio
