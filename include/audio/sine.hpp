#ifndef SINE_H
#define SINE_H

#include <portaudio.h>
#include <cassert>
#include <cmath>
#include <audio/table.hpp>
#include <fmt/ranges.h>

namespace Audio {

template <typename BufferType, std::size_t Channels>
class Sine {
public:
    using value_type = typename BufferType::value_type;
    using buffer_type = BufferType;

    void process(buffer_type& buffer) {
        for (auto& sample : buffer) {
            sample = table[phase++];
            if (phase > Size) {
                phase -= Size;
            }
        }
    }

private:
    int phase;

    static constexpr auto Size = 1024;

    static constexpr value_type PI2_DIV_SIZE = []() { return (M_PI * 2) / (Size * 4); }();
    static constexpr value_type sine(std::size_t index) {
        return 1 * std::sin(PI2_DIV_SIZE * static_cast<value_type>(index));
    }
    static constexpr std::array<value_type, Size> table = generate_table<value_type, Size>(&Sine::sine);
};

} // namespace Audio

#endif /* SINE_H */
