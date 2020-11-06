#include <algorithm>
#include <catch2/catch.hpp>
#include <audio/fft.hpp>
#include <complex>
#include <numeric>
#include <iostream>

TEST_CASE("Sanity") { REQUIRE(1 == 1); }

TEST_CASE("FFT") {
    SECTION("Forward fft, with peridoc signal") {
        std::array input{
#include "fft/periodicinput.txt"
        };
        std::array output{
#include "fft/periodicoutput.txt"
        };

        Audio::FFT<float, 5> fft{};
        auto fftdata = fft.createBuffer();

        fft.apply(input.data(), fftdata.data());

        // for (auto const v : fftdata) std::cout << v << std::endl;

        std::transform(fftdata.begin(), fftdata.end(), output.begin(), output.begin(),
                       [index = 0](auto const input, auto const output) mutable {
                           INFO("i=" << index++);
                           CHECK(input == output);
                           return output;
                       });
    }

    SECTION("Forward fft, with weird signal") {
        std::array<float, 16> input{
#include "fft/weirdinput.txt"
        };
        std::array<std::complex<float>, 16> output{
#include "fft/weirdoutput.txt"
        };

        Audio::FFT<float, 4> fft{};
        auto fftdata = fft.createBuffer();

        fft.apply(input.data(), fftdata.data());

        // for (auto const v : fftdata) std::cout << v << std::endl;

        std::transform(fftdata.begin(), fftdata.end(), output.begin(), output.begin(),
                       [index = 0](std::complex<float> const input, std::complex<float> const output) mutable {
                           INFO("i=" << index++);
                           CHECK(input.real() == Approx(output.real()).epsilon(0.005));
                           CHECK(input.imag() == Approx(output.imag()).epsilon(0.005));
                           return output;
                       });
    }
}
