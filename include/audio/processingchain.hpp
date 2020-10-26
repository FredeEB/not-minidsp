#ifndef SERIAL_H
#define SERIAL_H

#include <bits/c++config.h>
#include <tuple>
#include <type_traits>
#include <utility>

namespace Audio {

template <typename SystemTraits, typename... Algos>
class ProcessingChain {
public:
    using system_traits = SystemTraits;

    ProcessingChain(SystemTraits, Algos&... algos) : algorithmchain(algos...) {}

    template <typename BufferType>
    inline void process(BufferType& buffer) {
        process_impl(buffer, std::index_sequence_for<Algos...>());
    }

    template <std::size_t index>
    inline auto& getAlgo() {
        return std::get<index>(algorithmchain);
    }

private:
    template <typename BufferType, std::size_t... Idx>
    inline void process_impl(BufferType& buffer, std::index_sequence<Idx...>) {
        (..., std::get<Idx>(algorithmchain).process(buffer));
    }

    std::tuple<Algos&...> algorithmchain;
};

} // namespace Audio

#endif /* SERIAL_H */
