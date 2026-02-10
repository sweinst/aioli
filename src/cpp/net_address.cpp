#include <aioli/net_address.h>
#include <cstring>

namespace aioli {
    bool NetAddress::operator==(const NetAddress& other) const noexcept {
        return std::memcmp(&address_, &other.address_, sizeof(address_)) == 0;
    }

    std::string NetAddress::to_string() const noexcept {
        if (const auto* addr = std::get_if<sockaddr_in>(&address_); addr) {
            char buf[INET_ADDRSTRLEN];
            auto res = inet_ntop(AF_INET, &addr->sin_addr, buf, sizeof(buf));
            if (res) {
                return std::format("{}:{}", buf, ntohs(addr->sin_port));
            }
        } else if (const auto* addr6 = std::get_if<sockaddr_in6>(&address_); addr6) {
            char buf[INET6_ADDRSTRLEN];
            auto res = inet_ntop(AF_INET6, &addr6->sin6_addr, buf, sizeof(buf));
            if (res) {
                return std::format("[{}]:{}", buf, ntohs(addr6->sin6_port));
            }
        }
        return {};
    }

    NetAddress::NetAddress(const std::string& addr, int port) {
        sockaddr_in addr4{};
        sockaddr_in6 addr6{};
        if (inet_pton(AF_INET, addr.c_str(), &addr4.sin_addr) == 1) {
            addr4.sin_family = AF_INET;
            addr4.sin_port = static_cast<decltype(addr4.sin_port)>(port);
            address_ = addr4;
        } else if (inet_pton(AF_INET6, addr.c_str(), &addr6.sin6_addr) == 1) {
            addr6.sin6_family = AF_INET6;
            addr6.sin6_port = static_cast<decltype(addr6.sin6_port)>(port);
            address_ = addr6;
        } else {
            throw std::invalid_argument(get_net_error("Invalid IP address format: "s, addr));
        }
    }

    NetAddress::Raw NetAddress::get_raw_address() const {
        if (const auto* addr = std::get_if<sockaddr_in>(&address_); addr) {
            return {
                reinterpret_cast<const sockaddr*>(addr), static_cast<socklen_t>(sizeof(sockaddr_in))
            };
        } else if (const auto* addr6 = std::get_if<sockaddr_in6>(&address_); addr6) {
            return {
                reinterpret_cast<const sockaddr*>(addr6),
                static_cast<socklen_t>(sizeof(sockaddr_in6))
            };
        } else {
            throw std::runtime_error("NetAddress holds an invalid address");
        }
    }

    int NetAddress::get_family() const {
        if (std::holds_alternative<sockaddr_in>(address_)) {
            return AF_INET;
        } else if (std::holds_alternative<sockaddr_in6>(address_)) {
            return AF_INET6;
        } else {
            throw std::runtime_error("NetAddress holds an invalid address");
        }
    }  // namespace aio

}  // namespace aio
