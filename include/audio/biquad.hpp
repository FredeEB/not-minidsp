#ifndef BIQUAD_H
#define BIQUAD_H

#include <cmath>
#include <bits/c++config.h>
#include <portaudio.h>
#include <array>
#include <portaudiocpp/Stream.hxx>
#include <utility>

namespace Audio {

template <typename T, std::size_t FramesPerBuffer = 64, std::size_t Channels = 1>
class Biquad {
public:
    using value_type = T;
    Biquad() = default;
    Biquad(value_type ca1, value_type ca2, value_type cb0, value_type cb1, value_type cb2)
            : a1{ca1}, a2{ca2}, b0{cb0}, b1{cb1}, b2{cb2} {}
    void process(std::array<value_type, FramesPerBuffer>& buffer) {
        for (auto i = 0; i < FramesPerBuffer; ++i) {
            xn2 = xn1;
            xn1 = xn0;
            xn0 = buffer[i];

            buffer[i] = b0 * xn0 + b1 * xn1 + b2 * xn2 - a1 * yn1 - a2 * yn2;

            yn2 = yn1;
            yn1 = buffer[i];
        }
    }

private:
    value_type a1{}, a2{}, b0{}, b1{}, b2{};
    value_type yn1{}, yn2{}, xn0{}, xn1{}, xn2{};
};

} // namespace Audio

#endif /* BIQUAD_H */
