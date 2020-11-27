#ifndef FFT_H
#define FFT_H

#include <array>
#include <complex>

namespace Audio {

template <typename ValueType, std::size_t Size>
class FFT : FFT<ValueType, Size / 2> {
    // internal mixin
    using Next = FFT<ValueType, Size / 2>;
    // Imaginary unit: 0 + 1i
    static constexpr std::complex<ValueType> imag{0.0f, 1.0f};
    // precomputed 2 * pi * i
    static const inline auto TWO_PI_I = 2 * M_PIf32 * imag;

public:
    using complex_buffer_type = std::array<std::complex<ValueType>, Size>;
    complex_buffer_type static constexpr createBuffer() { return {}; }

    template <std::size_t Stride = 1>
    void constexpr apply(typename complex_buffer_type::pointer inbuffer,
                         typename complex_buffer_type::pointer outbuffer) {
        // Recursively call the Radix-2 DIT FFT
        Next::template apply<2 * Stride>(inbuffer, outbuffer);
        Next::template apply<2 * Stride>(inbuffer + Stride, outbuffer + Size / 2);

        std::size_t k{};
        for (std::size_t i = 0; i < Size / 2; ++i) {
            auto const exp = std::exp(-TWO_PI_I * (static_cast<ValueType>(k++) / Size));
            auto const temp = outbuffer[i];
            outbuffer[i] = temp + exp * outbuffer[i + Size / 2];
            outbuffer[i + Size / 2] = temp - exp * outbuffer[i + Size / 2];
        }
    }
    template <std::size_t Stride = 1>
    void constexpr inverse(typename complex_buffer_type::pointer inbuffer,
                           typename complex_buffer_type::pointer outbuffer) {
        Next::template inverse<2 * Stride>(inbuffer, outbuffer);
        Next::template inverse<2 * Stride>(inbuffer + Stride, outbuffer + Size / 2);

        std::size_t k{};
        for (std::size_t i = 0; i < Size / 2; ++i) {
            auto const exp = std::exp(TWO_PI_I * (static_cast<ValueType>(k++) / Size));
            auto const temp = outbuffer[i];
            outbuffer[i] = temp + exp * outbuffer[i + Size / 2];
            outbuffer[i + Size / 2] = temp - exp * outbuffer[i + Size / 2];
        }
    }
};

template <typename ValueType>
class FFT<ValueType, 1> {
public:
    using complex_buffer_type = std::array<std::complex<ValueType>, 1>;
    complex_buffer_type static constexpr createBuffer() { return {}; }
    template <std::size_t Stride>
    auto constexpr apply(typename complex_buffer_type::pointer inbuffer,
                         typename complex_buffer_type::pointer outbuffer) {
        outbuffer[0] = inbuffer[0];
    }
    template <std::size_t Stride = 1>
    auto constexpr inverse(typename complex_buffer_type::pointer inbuffer,
                           typename complex_buffer_type::pointer outbuffer) {
        outbuffer[0] = inbuffer[0] / static_cast<ValueType>(Stride);
    }
};

// template <std::size_t Size, std::size_t Stride = 1, std::size_t Start = 0, typename BufferType>
// auto fft(BufferType& buffer) {
//     using complex_value_type = typename BufferType::value_type;
//     using value_type = typename complex_value_type::value_type;
//     static constexpr typename BufferType::value_type imag{0.0f, 1.0f};
//     static const auto TWO_PI_I = 2 * M_PIf32 * imag;

//     if constexpr (Size == 1) {
//         // basecase, buffer[Start] = buffer[Start] * (1 + 0i), ie no change
//         return;
//     } else {
//         fft<Size / 2, Stride * 2, Start>(buffer);
//         fft<Size / 2, Stride * 2, Start + Size / 2>(buffer);

//         std::size_t k{};
//         for (auto i = Start; i < Start + Size / 2; ++i) {
//             auto const exp = std::exp(-TWO_PI_I * (static_cast<value_type>(k++) / Size));
//             auto const temp = buffer[i];
//             buffer[i] = temp + exp * buffer[i + Size];
//             buffer[i + Stride] = temp - exp * buffer[i + Size];
//         }
//     }
// }

} // namespace Audio

#endif /* FFT_H */
