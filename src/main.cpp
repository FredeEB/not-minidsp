#include <fmt/core.h>

#include <util/clihandler.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>
#include <audio/autosystem.hpp>
#include <audio/biquad.hpp>
#include <audio/fir.hpp>
#include <audio/processortraits.hpp>
#include <audio/processingchain.hpp>
#include <audio/parallel.hpp>

using namespace Audio;

int main(int argc, char** argv) {
    // initialize portaudio
    AutoSystem sys;


    SystemTraits<float, 512, 2> traits;

    auto& config = Util::getConfig();

    RoomCorrection<decltype(traits), FIRTag> rc(traits);
    rc.loadFiltersFromFile(config.FilterPath);

    ProcessingChain chain(traits, rc);

    AudioProcessor processor(chain);

    processor.run();

    while (true) {
    }

    return 0;
}
