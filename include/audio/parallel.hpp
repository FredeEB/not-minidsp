#ifndef CHANNELS_H
#define CHANNELS_H

#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <util/repeat_type.hpp>

namespace Audio {

template <typename... Algorithm>
class Parallel {
public:
    using algorithm_type = std::tuple<Algorithm...>;

    constexpr Parallel() = default;

    template <typename... Args>
    constexpr Parallel(Args... args) : algorithms(Algorithm(args...)...) {}

    template <typename BufferType>
    inline void process(BufferType& buffer) {
        process_impl(buffer, std::index_sequence_for<Algorithm...>());
    }

    template <std::size_t Index>
    inline auto get() noexcept {
        return std::get<Index>(algorithms);
    }

private:
    template <typename BufferType, std::size_t... idx>
    inline void process_impl(BufferType& buffer, std::index_sequence<idx...>) {
        (..., std::get<idx>(algorithms).process(std::get<idx>(buffer)));
    }

    algorithm_type algorithms;
};

template <typename Type, std::size_t Channels, typename... Args>
auto make_parallel(Args... args) {
    return typename Util::repeat_type<Type, Channels, Parallel>::type{args...};
}

} // namespace Audio

#endif /* CHANNELS_H */
