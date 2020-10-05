#ifndef CHANNELS_H
#define CHANNELS_H

#include <cstdint>
#include <array>
#include <functional>
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <utility>

namespace Audio {

template <typename, typename>
struct add_type {};

template <typename T, typename... Ts, template <typename...> typename TT>
struct add_type<T, TT<Ts...>> {
    using type = TT<Ts..., T>;
};

template <typename T, std::size_t N, template <typename...> typename TT>
struct repeat_type {
    using type = typename add_type<T, typename repeat_type<T, N - 1, TT>::type>::type;
};

template <typename T, template <typename...> typename TT>
struct repeat_type<T, 0, TT> {
    using type = TT<>;
};

template <typename... Algorithm>
class Parallel {
public:
    static_assert(std::is_same_v<typename Algorithm::buffer_type...>, "Buffers must be the same type");
    using algorithm_type = std::tuple<Algorithm...>;
    using buffer_type =
            typename std::remove_reference_t<decltype(std::get<0>(std::declval<algorithm_type>()))>::buffer_type;

    constexpr Parallel() = default;
    // template <typename... Args>
    // constexpr Parallel(Args... args) : algorithms(args...) {}

    void process(buffer_type& buffer) { process_impl(buffer, std::make_index_sequence<sizeof...(Algorithm)>()); }

private:
    template <std::size_t... Idx>
    void process_impl(buffer_type& buffer, std::index_sequence<Idx...>) {
        std::apply([&](auto& element) -> void { element.process(buffer); }, algorithms);
    }

    algorithm_type algorithms;
};

} // namespace Audio

#endif /* CHANNELS_H */
