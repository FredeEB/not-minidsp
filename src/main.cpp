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

    Util::parse_cli(argc, argv);

    SystemTraits<float, 256, 2> traits;

    auto& config = Util::getConfig();

    RoomCorrection<decltype(traits), FIRTag> rc;
    rc.loadFiltersFromFile(config.filterPath);

    ProcessingChain chain(traits, rc);

    AudioProcessor processor(chain);

    processor.run();

    while (true) {
    }

    return 0;
}
