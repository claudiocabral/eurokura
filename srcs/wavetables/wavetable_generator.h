#pragma once

#include <array>
#include <cstddef>

namespace kura {
template <size_t size, class Func>
constexpr auto make_wavetable(Func && f)
{
    using T = decltype(f(size, size));
    std::array<T, size + 3> output { T{} };
    for (size_t i = 0; i < size; ++i) output[i] = f(i, size);
    output[size + 0] = output[0];
    output[size + 1] = output[1];
    output[size + 2] = output[2];
    static_assert(output.size() == size + 3);
    return output;
}
}
