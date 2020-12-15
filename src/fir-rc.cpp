#include <util/clihandler.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>
#include <audio/autosystem.hpp>
#include <audio/biquad.hpp>
#include <audio/fir.hpp>
#include <audio/processortraits.hpp>
#include <audio/processingchain.hpp>
#include <audio/parallel.hpp>
#include <audio/fft.hpp>
#include <thread>
#include <audio/convolution.hpp>
#include <audio/processingchain.hpp>

using namespace Audio;

int main(int argc, char** argv) {
    Util::parse_cli(argc, argv);

    SystemTraits<float, 2048, 2> traits;

    auto& config = Util::getConfig();

    RoomCorrection<decltype(traits), ConvolutionTag> rc;

    ProcessingChain chain(traits, rc);
    if (config.filterPath.has_value()) chain.getAlgo<0>().loadFiltersFromFile(config.filterPath.value());

    AudioProcessor processor(chain);

    processor.run();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
