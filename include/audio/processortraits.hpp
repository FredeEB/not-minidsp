#ifndef PROCESSORTRAITS_H
#define PROCESSORTRAITS_H

#include <bits/c++config.h>
#include <array>
#include <tuple>
#include "util/repeat_type.hpp"
template <typename ValueType, std::size_t ChannelSize, std::size_t Channels>
struct SystemTraits {
    static constexpr auto channels = Channels;
    static constexpr auto channel_size = ChannelSize;
    static constexpr auto frame_size = channels * channel_size;
    static_assert(frame_size > 0, "Framesize can't be 0, both channels and channel_size has to be greater than 0");
    using value_type = ValueType;
    using channel_type = std::array<value_type, channel_size>;
    using buffer_type = std::array<channel_type, channels>;
};

#endif /* PROCESSORTRAITS_H */
