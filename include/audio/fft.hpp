#ifndef FFT_H
#define FFT_H

#include <array>
#include <cmath>
#include <iterator>
#include <complex>
#include <tuple>
#include <iostream>

namespace Audio {

template <typename ValueType, std::size_t P>
class FFT {
    std::size_t static constexpr N = 1 << P;
    FFT<ValueType, P - 1> next;

    static constexpr std::complex<ValueType> imag{0.0f, 1.0f};
    static const inline auto TWO_PI_I = 2 * M_PIf32 * imag;

public:
    using ComplexBufferType = std::array<std::complex<ValueType>, N>;

    ComplexBufferType static constexpr createBuffer() { return {}; }

    template <std::size_t S = 1>
    void constexpr apply(ValueType* inbuffer, typename ComplexBufferType::pointer outbuffer) {
        // Recursively call the Radix-2 DIT FFT
        next.template apply<2 * S>(inbuffer, outbuffer);
        next.template apply<2 * S>(inbuffer + S, outbuffer + N / 2);

        for (auto i = 0; i < N / 2; ++i) {
            auto exp = std::exp(-TWO_PI_I * (static_cast<float>(i) / N));
            auto temp = outbuffer[i];
            outbuffer[i] = temp + exp * outbuffer[i + N / 2];
            outbuffer[i + N / 2] = temp - exp * outbuffer[i + N / 2];
        }
    }
};

template <typename ValueType>
class FFT<ValueType, 0> {
public:
    using ComplexBufferType = std::array<std::complex<ValueType>, 1>;
    ComplexBufferType static constexpr createBuffer() { return {}; }
    template <std::size_t S>
    auto constexpr apply(ValueType* inbuffer, typename ComplexBufferType::pointer outbuffer) {
        outbuffer[0] = inbuffer[0];
    }
};

} // namespace Audio

#endif /* FFT_H */
