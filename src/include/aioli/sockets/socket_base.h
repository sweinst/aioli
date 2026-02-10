#pragma once
#include "../sockets/socket_types.h"
#include "../event_pollers/event_poller_base.h"

namespace aioli {
    /** Common code for all sockets which is not socket type specific. 
     * Here sockets can be network sockets (TCP, UDP) or "file sockets" (pipes, stdin, ...).
     */
    class SocketBase {
    protected:
        SocketBase() noexcept = default;
        SocketBase(EventPollerBase& event_poller, SOCKET fd) noexcept
        : event_poller_(&event_poller), fd_(fd) {}
        SocketBase(EventPollerBase& event_poller, int domain, int type)
        : event_poller_(&event_poller), fd_(create(domain, type)) {}

        /*! create the socket */
        SOCKET create(int domain, int type);
        /*! Sets up the socket with common options (non-blocking mode, keep-alive, etc.) */
        SOCKET setup(SOCKET fd);
    private:
        EventPollerBase *event_poller_ = nullptr;
        SOCKET fd_{ INVALID_SOCKET };
    };
}
