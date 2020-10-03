#include <fmt/core.h>

#include <chrono>
#include <thread>
#include <util/config.hpp>
#include <util/singleton.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>
#include <audio/sine.hpp>
#include <audio/biquad.hpp>

int main() {
    Audio::AudioProcessor<Audio::RoomCorrection, Audio::FIRTag> processor(1024UL);

    processor.algo().loadFilterFromFile("filter.txt");
    processor.run();
    while (true) {
    }

    return 0;
}
