#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <bits/c++config.h>
#include <fmt/os.h>
#include <memory>
#include <portaudiocpp/Device.hxx>
#include <portaudiocpp/SampleDataFormat.hxx>
#include <portaudiocpp/System.hxx>
#include <portaudiocpp/Stream.hxx>
#include <portaudiocpp/StreamParameters.hxx>
#include <portaudiocpp/DirectionSpecificStreamParameters.hxx>
#include <portaudiocpp/BlockingStream.hxx>
#include <util/config.hpp>
#include <util/singleton.hpp>
#include <thread>

namespace Audio {

template <typename Algorithm, std::size_t FramesPerBuffer = 64, std::size_t Channels = 2>
class AudioProcessor {
public:
    using value_type = typename Algorithm::value_type;

    void run() {
        auto& sys = portaudio::System::instance();
        auto& config = Util::Singleton<Util::Config>();
        portaudio::DirectionSpecificStreamParameters inputparams(
                sys.deviceByIndex(config.deviceIndex), Channels, portaudio::INT24, false,
                sys.defaultInputDevice().defaultLowInputLatency(), nullptr);
        portaudio::DirectionSpecificStreamParameters outputparams(
                sys.deviceByIndex(config.deviceIndex), Channels, portaudio::INT24, false,
                sys.defaultOutputDevice().defaultLowOutputLatency(), nullptr);

        portaudio::StreamParameters params(inputparams, outputparams, config.SampleRate, FramesPerBuffer, paClipOff);
        stream.open(params);
        process = std::thread([this] {
            stream.start();
            running = true;
            while (running) {
                stream.read(static_cast<void*>(buffer.data()), buffer.size());
                algorithm.process(buffer);
                stream.write(static_cast<void*>(buffer.data()), buffer.size());
            }
            stream.stop();
        });
    }

    void stop() { running = false; }

    Algorithm& algo() { return algorithm; }

private:
    std::array<value_type, FramesPerBuffer> buffer;
    portaudio::BlockingStream stream;
    Algorithm algorithm;
    std::thread process;
    bool running{false};
};

} // namespace Audio

#endif /* AUDIOPROCESSOR_H */
