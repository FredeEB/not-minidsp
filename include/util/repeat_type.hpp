#ifndef REPEAT_TYPE_H
#define REPEAT_TYPE_H

#include <cstdint>

namespace Util {

template <typename TypeName, typename TypeList>
struct add_type {};

template <typename TypeName, typename... Rest, template <typename...> typename TypeList>
struct add_type<TypeName, TypeList<Rest...>> {
    using Type = TypeList<Rest..., TypeName>;
};

template <typename TypeName, std::size_t N, template <typename...> typename TypeList>
struct repeat_type {
    using Type = typename add_type<TypeName, typename repeat_type<TypeName, N - 1, TypeList>::Type>::Type;
};

template <typename TypeName, template <typename...> typename TypeList>
struct repeat_type<TypeName, 0, TypeList> {
    using Type = TypeList<>;
};

} // namespace Util

#endif /* REPEAT_TYPE_H */
