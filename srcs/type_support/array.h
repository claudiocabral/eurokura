#pragma once

#include <array>
#include <type_traits>
#include <initializer_list>

namespace tak {
template <class T, class... Args>
constexpr auto make_array(Args&&... args) {
    return std::array<T, sizeof...(args)>{ args... };
}
}
