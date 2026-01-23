#pragma once

#include <coroutine>
#include <chrono>
#include <expected>
#include <print>

namespace aio {
    using namespace std::chrono_literals;
    namespace chrono = std::chrono;

    using id_t = uint64_t;
    constexpr id_t INVALID_ID = static_cast<id_t>(-1);

    using coro_hdl = std::coroutine_handle<>;

    using clock = std::chrono::steady_clock;
    using time_point = clock::time_point;
    using duration = clock::duration;
} // namespace aio

#include "base_net.h"
#include "timer.h"
#include "events.h"
#include "event_loop.h"

