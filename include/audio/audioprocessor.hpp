#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <algorithm>
#include <array>
#include <memory>
#include <stdexcept>
#include <thread>

#include <util/config.hpp>
#include <util/numbertype.hpp>
#include <util/singleton.hpp>

#include <portaudio.h>

namespace Audio {
template <template <typename BufferType, std::size_t Channels, typename... Tag> typename Algorithm, typename Tag,
          typename BufferType = std::array<float, 32>, std::size_t Channels = 1>
class AudioProcessor {
public:
    using value_type = typename BufferType::value_type;
    using algorithm_type = Algorithm<BufferType, Channels, Tag>;
    using buffer_type = BufferType;

    template <typename... Args>
    AudioProcessor(Args... args) : algorithm(args...) {
        auto err = Pa_Initialize();
        if (err != paNoError) throw std::runtime_error("Failed to initialize Portaudio");
    }
    ~AudioProcessor() noexcept {
        stop();
        Pa_Terminate();
    }

    void run() {
        auto err =
                Pa_OpenDefaultStream(&stream, Channels, Channels, Util::NumberType<value_type>::value,
                                     Util::Singleton<Util::Config>().SampleRate, buffer.size(), StreamCallback, this);
        if (err != paNoError) throw std::runtime_error("Failed to open stream");
        Pa_StartStream(stream);
    }

    inline void stop() noexcept {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }

    inline algorithm_type& algo() { return algorithm; }

private:
    // Callback function for audiostream. Will call process on algorithm
    inline static int StreamCallback(const void* input, void* output, unsigned long frameCount,
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
