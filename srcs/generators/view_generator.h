#pragma once
#include <ranges>
#include <memory>

// goal: this syntax or something like it
// oscillator | envelope | vca("freq", oscillator) | output;
//
template <typename t>
using iterator_t = decltype(std::begin(std::declval<t &>()));

template <typename t>
using range_reference_t = decltype(*std::begin(std::declval<t &>()));

template<typename urng_t>
struct GeneratorIterator : std::ranges::view_base
{
    public:
        struct iterator_type : iterator_t<urng_t>
    {
        using base = iterator_t<urng_t>;
        using reference = uint64_t;

        iterator_type() = default;
        iterator_type(base const & b) : base{b} {}

        iterator_type operator++(int)
        {
            return static_cast<base&>(*this)++;
        }

        iterator_type & operator++()
        {
            ++static_cast<base&>(*this);
            return (*this);
        }

        reference operator*() const
        {
            return *static_cast<base>(*this) + 42;
        }
    };
        /* member type definitions */
        using reference         = uint64_t;
        using const_reference   = uint64_t;
        using value_type        = uint64_t;
        using iterator          = iterator_type;
        using const_iterator    = iterator_type;
        /* constructors and deconstructors */
        GeneratorIterator() = default;
        constexpr GeneratorIterator(GeneratorIterator const & rhs) = default;
        constexpr GeneratorIterator(GeneratorIterator && rhs) = default;
        constexpr GeneratorIterator & operator=(GeneratorIterator const & rhs) = default;
        constexpr GeneratorIterator & operator=(GeneratorIterator && rhs) = default;
        ~GeneratorIterator() = default;
        GeneratorIterator(urng_t && urange, int i)
            : urange{std::forward<urng_t>(urange)},
              i{i}
        {}

        auto begin() const
        {
            return std::begin(urange);
        }

        auto cbegin() const
        {
            return begin();
        }

        auto end() const
        {
            return std::end(urange);
        }

        auto cend() const
        {
            return end();
        }
        urng_t urange;
        int i;
};

struct add_constant_fn
{
    int i = 7;
    template <typename urng_t>
        //         requires (bool)ranges::InputRange<urng_t>() &&
        //                  (bool)ranges::CommonReference<range_reference_t<urng_t>, uint64_t>()
        auto operator()(urng_t && urange) const
        {
            return GeneratorIterator<urng_t>{std::forward<urng_t>(urange)};
        }

    template <typename urng_t>
        //         requires (bool)ranges::InputRange<urng_t>() &&
        //                  (bool)ranges::CommonReference<range_reference_t<urng_t>, uint64_t>()
        friend auto operator|(urng_t && urange, add_constant_fn const &self)
        {
            return GeneratorIterator<urng_t>{std::forward<urng_t>(urange), self.i};
        }

};

add_constant_fn constexpr inline add_constant;


void f()
{
    std::vector<uint64_t> in{1, 4, 6, 89, 56, 45, 7};
    auto v = in | add_constant_fn{3};
}
