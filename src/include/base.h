#pragma once

#include "base_net.h"
#include <coroutine>
#include <chrono>
#include <expected>
#include <print>

using namespace std::chrono_literals;

namespace aio {
    using coro_handle = std::coroutine_handle<>;
    using clock = std::chrono::steady_clock;
    using time_point = clock::time_point;
} // namespace aio
