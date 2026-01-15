#pragma once

#include "../base.h"

namespace aioli {
    /** Concept for operations depending on the type of sockets */
    template <class SocketOperations>
    concept SocketOperations_t = requires(SocketOperations op, SOCKET fd, void* buf, size_t count) {
        { SocketOperations::Read(fd, buf, count) } -> std::convertible_to<int>;
        { SocketOperations::Write(fd, buf, count) } -> std::convertible_to<int>;
        { SocketOperations::Close(fd) } -> std::convertible_to<int>;
    };

}  // namespace aioli
