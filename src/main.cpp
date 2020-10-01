#include <fmt/core.h>

#include <util/config.hpp>
#include <util/singleton.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>
#include <audio/sine.hpp>
#include <audio/biquad.hpp>

using namespace std::literals::chrono_literals;

static const std::size_t FramesPerBuffer = 512;
static const std::size_t Channels = 2;

int main() {
    Audio::AudioProcessor<Audio::RoomCorrection, Audio::BiquadTag> processor;

    processor.algo().registerFilter({-1.96297470f, 0.96364759f, 0.000168223247f, 0.000336446494f, 0.000168223247f});
    processor.algo().registerFilter({-1.81531792f, 0.83098222f, 0.00391608f, 0.00783215f, 0.00391608f});
    processor.run();
    std::this_thread::sleep_for(5s);

    return 0;
}
