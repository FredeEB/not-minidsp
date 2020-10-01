#include <fmt/core.h>

#include <util/config.hpp>
#include <util/singleton.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>
#include <audio/sine.hpp>
#include <audio/fir.hpp>

using namespace std::literals::chrono_literals;

static const std::size_t FramesPerBuffer = 512;
static const std::size_t Channels = 2;

int main() {
    Audio::AudioProcessor<Audio::RoomCorrection, Audio::FIRTag> processor(1024);

    processor.run();
    std::this_thread::sleep_for(5s);

    return 0;
}
