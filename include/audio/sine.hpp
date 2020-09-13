#ifndef SINE_H
#define SINE_H

#include <portaudio.h>
#include <cassert>
#include <cmath>
#include <audio/table.hpp>
#include <fmt/ranges.h>

namespace Audio {

template <int Channels = 1, std::size_t Size = 256>
class Sine {
public:
    int generate(void const *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                 PaStreamCallbackTimeInfo const *, PaStreamCallbackFlags) {
        assert(inputBuffer != nullptr);

        auto out = static_cast<float **>(outputBuffer);

        for (auto channel = 0; channel < Channels; ++channel) {
            for (auto i = 0; i < framesPerBuffer; ++i) {
                out[channel][i] = table[phase[channel]++];
                if (phase[channel] > Size) {
                    phase[channel] -= Size;
                }
            }
        }
        return paContinue;
    }

private:
    std::array<int, Channels> phase;

    static constexpr double PI2_DIV_SIZE = []() { return (M_PI * 2) / (Size * 4); }();
    static constexpr double sine(std::size_t index) { return 1 * std::sin(PI2_DIV_SIZE * static_cast<double>(index)); }
    static constexpr std::array<double, Size> table = generate_table<double, Size>(&Sine::sine);
};

} // namespace Audio

#endif /* SINE_H */
