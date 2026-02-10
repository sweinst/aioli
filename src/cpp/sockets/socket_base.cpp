#include <aioli/sockets/socket_base.h>

namespace aioli {
    SOCKET SocketBase::create(int domain, int type) {
        auto s = socket(domain, type, 0);
        if (s == INVALID_SOCKET) {
            throw net_error("Unable to create socket: "s);
        }
        return setup(s);
    }

    SOCKET SocketBase::setup(SOCKET fd) {
        int socket_type;
        socklen_t len = sizeof(socket_type);
        if (getsockopt(fd, SOL_SOCKET, SO_TYPE, (char*)&socket_type, &len) == 0) {
            if (socket_type == SOCK_STREAM) {
                // Enable TCP keep-alive
                int optval = 1;
                if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&optval, sizeof(optval)) < 0) {
                    throw net_error("Unable to enable keep-alive: "s);
                }
            }
            // set non-blocking mode
            #ifdef _WIN32
                u_long mode = 1;
                if (ioctlsocket(fd, FIONBIO, &mode) != 0) {
                    throw net_error("Unable to set non-blocking mode: "s);
                }
            #else
                auto flags = fcntl(fd, F_GETFL, 0);
                if (flags < 0) {
                    throw net_error("Unable to get socket flags: "s);
                }
                if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
                    throw net_error("Unable to set non-blocking mode: "s);
                }
            #endif
        }
        return fd;
    }
}  // namespace aioli
