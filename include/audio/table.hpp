#ifndef TABLE_H
#define TABLE_H

#include <cstdint>
#include <array>

template <typename ValueType, typename LambdaType, std::size_t... Idx>
constexpr std::array<ValueType, sizeof...(Idx)> generate_table(std::index_sequence<Idx...>, LambdaType function) {
    return {function(Idx)...};
}

template <typename ValueType, std::size_t N, typename LambdaType>
constexpr std::array<ValueType, N> generate_table(LambdaType function) {
    return generate_table<ValueType>(std::make_index_sequence<N>(), function);
}

#endif /* TABLE_H */
