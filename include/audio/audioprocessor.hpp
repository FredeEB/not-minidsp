#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <bits/c++config.h>
#include <fmt/core.h>
#include <portaudio.h>
#include <algorithm>
#include <array>
#include <memory>
#include <portaudiocpp/Device.hxx>
#include <portaudiocpp/System.hxx>
#include <portaudiocpp/StreamParameters.hxx>
#include <portaudiocpp/DirectionSpecificStreamParameters.hxx>
#include <portaudiocpp/BlockingStream.hxx>
#include <stdexcept>
#include <util/config.hpp>
#include <util/numbertype.hpp>
#include <util/singleton.hpp>
#include <audio/audiostream.hpp>
#include <thread>

namespace Audio {
template <template <typename BufferType, std::size_t Channels> typename Algorithm,
          typename BufferType = std::array<float, 512>, std::size_t Channels = 2>
class AudioProcessor {
public:
    using value_type = typename BufferType::value_type;
    using algorithm_type = Algorithm<BufferType, Channels>;
    using buffer_type = BufferType;

    AudioProcessor() {
        auto err = Pa_Initialize();
        if (err != paNoError) throw std::runtime_error("Failed to initialize Portaudio");
    }
    ~AudioProcessor() { Pa_Terminate(); }

    void run() {
        auto err =
                Pa_OpenDefaultStream(&stream, Channels, Channels, Util::NumberType<value_type>::value,
                                     Util::Singleton<Util::Config>().SampleRate, buffer.size(), StreamCallback, this);
        if (err != paNoError) throw std::runtime_error("Failed to open stream");
        fmt::print("Opened stream\n");
        Pa_StartStream(stream);
    }

    void stop() {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }

    algorithm_type& algo() { return algorithm; }

private:
    static int StreamCallback(const void* input, void* output, unsigned long frameCount,
                              const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* object) {
        auto self = static_cast<AudioProcessor*>(object);
        auto inbuffer = static_cast<value_type const*>(input);
        auto outbuffer = static_cast<value_type*>(output);
        for (std::size_t i = 0; i < frameCount; ++i) {
            self->buffer[i] = inbuffer[i];
        }
        self->algorithm.process(self->buffer);
        for (std::size_t i = 0; i < frameCount; ++i) {
            outbuffer[i] = self->buffer[i];
        }
        return paContinue;
    }

    buffer_type buffer;
    PaStream* stream{};
    algorithm_type algorithm;
    std::thread process;
    bool running{false};
};

} // namespace Audio

#endif /* AUDIOPROCESSOR_H */
