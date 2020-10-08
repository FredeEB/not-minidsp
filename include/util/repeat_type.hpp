#ifndef REPEAT_TYPE_H
#define REPEAT_TYPE_H

#include <cstdint>

namespace Util {

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

} // namespace Util

#endif /* REPEAT_TYPE_H */
