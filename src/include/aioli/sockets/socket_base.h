#pragma once
#include "../sockets/socket_types.h"
#include "../event_pollers/event_poller_base.h"

namespace aioli {
    /** Common code for all sockets which is not socket type specific. 
     * Here sockets can be network sockets (TCP, UDP) or "file sockets" (pipes, stdin, ...).
     */
    class SocketBase {
    public:
        explicit SocketBase(SOCKET fd) noexcept : fd_(fd) {}
    private:
        EventPollerBase *event_poller_ = nullptr;
        SOCKET fd_{ INVALID_SOCKET };
    };
}
