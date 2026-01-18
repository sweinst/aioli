#pragma once
/**
 * Platform independent network includes and definitions
 */


#ifdef _WIN32
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <io.h>
#else
#   include <arpa/inet.h>
#   include <errno.h>
#   include <netdb.h>
#   include <sys/socket.h>
#   include <unistd.h>
#   include <string.h>
#endif
#include <expected>
#include <format>
using namespace std::string_literals;

#ifdef _WIN32
    typedef int socklen_t;

    /** Wrapper to get last network error message */
    template<typename... Args>
    inline std::string get_net_error(const std::string& msg, Args... args) noexcept {
        char buf[256];
        strerror_s(buf, sizeof(buf), WSAGetLastError());
        // NB: in C++26, std::runtime_format will simplify this by allowing to pass a dynamic format string
        return std::format("{} Error: {}", std::vformat(msg, std::make_format_args(args...)), buf);
    }

    /** Initialize network libraries */
    inline std::expected<bool, std::string> initialize_net() noexcept {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            return std::unexpected(get_net_error("WSAStartup failed"s) );
        }
        return true;
    }

    /** Cleanup network libraries */
    inline bool cleanup_net() noexcept {
        WSACleanup();
        return true;
    }
#else
    /** common definitions for all platforms */
    using SOCKET = int;
    consteval SOCKET INVALID_SOCKET = -1;

    /** Wrapper to get last network error message */
    template<typename... Args>
    inline std::string get_net_error(const std::string& msg, Args... args) noexcept {
        // NB: in C++26, std::runtime_format will simplify this by allowing to pass a dynamic format string
        return std::format("{} Error: {}", std::vformat(msg, std::make_format_args(args...)), strerror(errno));
    }

    /** Initialize network libraries */
    inline  std::expected<bool, std::string> initialize_net() {
        return true;
    }

    /** Cleanup network libraries */
    inline bool cleanup_net() noexcept {
        return true;
    }

    /** allow to use the same function name on all platforms */
    inline int closesocket(SOCKET fd) noexcept {
        return close(fd);
    }
#endif

