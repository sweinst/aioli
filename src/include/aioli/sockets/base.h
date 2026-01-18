#pragma once
#include "../sockets/base.h"
#include "../sockets/types.h"

namespace aioli {
    /** Common code for all sockets which is not socket type specific. 
     * Here sockets can be network sockets (TCP, UDP) or "file sockets" (pipes, stdin, ...).
     */
    class SocketBase {
    public:
        explicit SocketBase(SOCKET fd) noexcept : fd_(fd) {}
    private:
        SOCKET fd_{ INVALID_SOCKET };
    };
}
