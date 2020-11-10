#include <algorithm>
#include <catch2/catch.hpp>
#include <audio/fft.hpp>
#include <complex>
#include <numeric>
#include <iostream>

TEST_CASE("Sanity") { REQUIRE(1 == 1); }

TEST_CASE("Forward FFT") {
    SECTION("with peridoc signal") {
        std::array input{
#include "fft/periodictime.txt"
        };
        std::array output{
#include "fft/periodicfrequency.txt"
        };

        Audio::FFT<float, 32> fft{};
        auto fftdatain = fft.createBuffer();
        auto fftdataout = fft.createBuffer();
        std::copy(input.begin(), input.end(), fftdatain.begin());
        fft.apply(fftdatain.data(), fftdataout.data());

        std::transform(fftdataout.begin(), fftdataout.end(), output.begin(), output.begin(),
                       [index = 0](auto const input, auto const output) mutable {
                           INFO("i=" << index++);
                           CHECK(input == output);
                           return output;
                       });
    }

    SECTION("with weird signal") {
        std::array input{
#include "fft/weirdtime.txt"
        };
        std::array output{
#include "fft/weirdfrequency.txt"
        };

        Audio::FFT<float, 16> fft{};
        auto fftdatain = fft.createBuffer();
        auto fftdataout = fft.createBuffer();
        std::copy(input.begin(), input.end(), fftdatain.begin());
        fft.apply(fftdatain.data(), fftdataout.data());

        std::transform(fftdataout.begin(), fftdataout.end(), output.begin(), output.begin(),
                       [index = 0](auto const input, auto const output) mutable {
                           INFO("i=" << index++);
                           CHECK(input.real() == Approx(output.real()).epsilon(0.005));
                           CHECK(input.imag() == Approx(output.imag()).epsilon(0.005));
                           return output;
                       });
    }
}

TEST_CASE("Inverse FFT") {
    SECTION("with peridoc signal") {
        std::array input{
#include "fft/periodicfrequency.txt"
        };
        std::array output{
#include "fft/periodictime.txt"
        };

        Audio::FFT<float, 32> fft{};
        auto fftdatain = fft.createBuffer();
        auto fftdataout = fft.createBuffer();
        std::copy(input.begin(), input.end(), fftdatain.begin());
        fft.inverse(fftdatain.data(), fftdataout.data());

        std::transform(fftdataout.begin(), fftdataout.end(), output.begin(), output.begin(),
                       [index = 0](auto const input, auto const output) mutable {
                           INFO("i=" << index++);
                           CHECK(input.real() == output);
                           return output;
                       });
    }

    SECTION("with weird signal") {
        std::array input{
#include "fft/weirdfrequency.txt"
        };
        std::array output{
#include "fft/weirdtime.txt"
        };

        Audio::FFT<float, 16> fft{};
        auto fftdatain = fft.createBuffer();
        auto fftdataout = fft.createBuffer();
        std::copy(input.begin(), input.end(), fftdatain.begin());
        fft.inverse(fftdatain.data(), fftdataout.data());

        std::transform(fftdataout.begin(), fftdataout.end(), output.begin(), output.begin(),
                       [index = 0](auto const input, auto const output) mutable {
                           INFO("i=" << index++);
                           CHECK(input.real() == Approx(output).margin(0.005));
                           CHECK(input.imag() == Approx(0).margin(0.005));
                           return output;
                       });
    }
}
