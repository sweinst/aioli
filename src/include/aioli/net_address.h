#pragma once
#include <variant>
#include "base.h"

namespace aioli {
    /**
     * Represents a network address (IPv4 or IPv6)
     */
    class NetAddress {
       public:
        using Raw = std::pair<const sockaddr*, socklen_t>;

        NetAddress() = default;
        NetAddress(const NetAddress&) = default;
        NetAddress& operator=(const NetAddress&) = default;
        NetAddress(NetAddress&&) noexcept = default;
        NetAddress& operator=(NetAddress&&) noexcept = default;

        NetAddress(const std::string& addr, int port);
        NetAddress(const sockaddr_in& addr)
            : address_{addr} {}
        NetAddress(const sockaddr_in6& addr)
            : address_{addr} {}

        ~NetAddress() = default;

        bool operator==(const NetAddress& other) const noexcept;
        std::string to_string() const noexcept;

        Raw get_raw_address() const;
        int get_family() const;

       private:
        std::variant<sockaddr_in, sockaddr_in6> address_;
    };

}  // namespace aio
