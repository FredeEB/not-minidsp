#include <fmt/core.h>

#include <util/config.hpp>
#include <util/singleton.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>
#include <audio/sine.hpp>
#include <audio/biquad.hpp>

using namespace std::literals::chrono_literals;

int main() {
    Audio::AudioProcessor<Audio::RoomCorrection, Audio::BiquadTag> processor;

    processor.algo().loadFiltersFromFile("filters.txt");
    processor.algo().printfilters(std::cout);

    processor.run();
    while (true) {
    }

    return 0;
}
