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
	using fft_type = FFT<value_type, Size>;
    using complex_buffer_type = typename fft_type::complex_buffer_type;

    constexpr ConvolutionFilter() noexcept { filter.fill(1); };
    constexpr ConvolutionFilter(complex_buffer_type coef) noexcept : timebuffer{std::move(coef)} {
        fft.apply(timebuffer.data(), filter.data());
    }

    void process(buffer_type& buffer) noexcept {
		std::copy(buffer.cbegin(), buffer.cend(), timebuffer.begin());

        fft.apply(timebuffer.data(), freqbuffer.data());

        std::transform(freqbuffer.begin(), freqbuffer.end(), filter.cbegin(), freqbuffer.begin(),
                       [](auto const value1, auto const value2) { return value1 * value2; });

        fft.inverse(freqbuffer.data(), timebuffer.data());

		for(std::size_t i = 0; i < buffer.size(); ++i)
			buffer[i] = timebuffer[i].real() + old[i];
		for(std::size_t i = buffer.size(); i < timebuffer.size(); ++i){
			old[i-buffer.size()] = timebuffer[i].real();
			timebuffer[i] = {};
		}
    }

private:
	fft_type fft{};
    buffer_type old{};
    complex_buffer_type filter{};
    complex_buffer_type timebuffer{};
    complex_buffer_type freqbuffer{};
};

} // namespace Audio

#endif /* CONVOLUTION_H */
