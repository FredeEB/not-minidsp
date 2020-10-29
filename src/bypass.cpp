#include <util/clihandler.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>
#include <audio/autosystem.hpp>
#include <audio/biquad.hpp>
#include <audio/fir.hpp>
#include <audio/processortraits.hpp>
#include <audio/processingchain.hpp>
#include <audio/parallel.hpp>
#include <audio/passthrough.hpp>

using namespace Audio;

int main(int argc, char** argv) {
    // initialize portaudio
    AutoSystem sys;

    Util::parse_cli(argc, argv);

    SystemTraits<float, 256, 2> traits;

    Passthrough<decltype(traits)> pt;

    ProcessingChain chain(traits, pt);

    AudioProcessor processor(chain);

    processor.run();

    while (true) {
    }

    return 0;
}
