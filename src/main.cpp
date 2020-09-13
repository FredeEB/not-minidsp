#include <fmt/core.h>
#include <portaudiocpp/AutoSystem.hxx>
#include <iostream>

#include <portaudiocpp/System.hxx>
#include <util/config.hpp>
#include <util/print.hpp>
#include <util/singleton.hpp>
#include <audio/audioprocessor.hpp>
#include <audio/roomcorrection.hpp>

static const std::size_t FramesPerBuffer = 64;
static const std::size_t Channels = 2;

void setup() {
    fmt::print("Running setup...\n");
    auto& config = Util::Singleton<Util::Config>();

    print_devices();
    config.SampleRate = 48000.0;

    fmt::print("Which card to use?\n");
    std::cin >> config.deviceIndex;

    fmt::print("------------\n");
    fmt::print("Device: {}\n", portaudio::System::instance().deviceByIndex(config.deviceIndex).name());
    fmt::print("Channels: {}\n", Channels);
    fmt::print("Frames per buffer: {}\n", FramesPerBuffer);
    fmt::print("Samplerate: {}\n", config.SampleRate);
    fmt::print("------------\n");
}

int main() {
    portaudio::AutoSystem system;
    setup();

    Audio::AudioProcessor<Audio::RoomCorrection<float>> processor;

    processor.algo().registerFilter({1, 0.2f, 1, 1, 1});
    processor.run();

    return 0;
}
