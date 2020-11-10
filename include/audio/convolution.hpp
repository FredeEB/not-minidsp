#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <algorithm>

#include <audio/fft.hpp>

namespace Audio {

struct ConvolutionTag {};

template <std::size_t Size, typename BufferType>
class ConvolutionFilter {
public:
    using buffer_type = BufferType;
    using value_type = typename buffer_type::value_type;
    using complex_buffer_type = typename FFT<value_type, Size>::complex_buffer_type;

    constexpr ConvolutionFilter() noexcept { filter.fill(1); };
    constexpr ConvolutionFilter(complex_buffer_type coef) noexcept : timebuffer{std::move(coef)} {
        fft.apply(timebuffer.data(), filter.data());
    }

    void process(buffer_type& buffer) {
        std::copy(buffer.begin(), buffer.end(), timebuffer.begin());
        fft.apply(timebuffer.data(), freqbuffer.data());

        std::transform(freqbuffer.begin(), freqbuffer.end(), filter.cbegin(), freqbuffer.begin(),
                       [](auto const value1, auto const value2) { return value1 * value2; });

        fft.inverse(freqbuffer.data(), timebuffer.data());
        std::transform(timebuffer.begin(), timebuffer.end(), buffer.begin(),
                       [](auto const value) { return value.real(); });
    }

private:
    FFT<value_type, Size> fft{};
    complex_buffer_type filter{};
    complex_buffer_type timebuffer{};
    complex_buffer_type freqbuffer{};
};

} // namespace Audio

#endif /* CONVOLUTION_H */
