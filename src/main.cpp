#include <fmt/core.h>

#include <chrono>
#include <thread>
#include <util/config.hpp>
#include <util/singleton.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>
#include <audio/sine.hpp>
#include <audio/autosystem.hpp>
#include <audio/biquad.hpp>
int main(int argc, char** argv) {
    // initialize portaudio
    AutoSystem sys;


    processor.algo().loadFilterFromFile("filter.txt");
    processor.run();
    while (true) {
    }

    return 0;
}
