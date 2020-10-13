#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <algorithm>
#include <array>
#include <cstring>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <utility>

#include <util/config.hpp>
#include <util/numbertype.hpp>
#include <util/singleton.hpp>
#include <util/repeat_type.hpp>
#include <audio/parallel.hpp>
#include <audio/processortraits.hpp>

#include <fmt/core.h>
#include <portaudio.h>
#include <iostream>

namespace Audio {
template <template <typename SystemTraits, typename... Tag> typename Algorithm, typename Tag,
          typename SystemTraits = SystemTraits<float, 64, 2>>
class AudioProcessor {
public:
    using value_type = typename SystemTraits::value_type;
    using channel_type = typename SystemTraits::channel_type;
    using buffer_type = typename SystemTraits::buffer_type;
    using algorithm_type = Algorithm<SystemTraits, Tag>;

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
        PaDeviceInfo const* dev = Pa_GetDeviceInfo(Pa_GetDeviceCount() - 1);
        if (dev->maxInputChannels < SystemTraits::channels)
            throw std::runtime_error("Device doesn't have anough input channels for requested configuration");
        if (dev->maxOutputChannels < SystemTraits::channels)
            throw std::runtime_error("Device doesn't have anough output channels for requested configuration");

        PaStreamParameters inparams = {Pa_GetDeviceCount() - 1, SystemTraits::channels,
                                       Util::NumberType<value_type>::value | paNonInterleaved,
                                       dev->defaultHighInputLatency, nullptr};
        PaStreamParameters outparams = {Pa_GetDeviceCount() - 1, SystemTraits::channels,
                                        Util::NumberType<value_type>::value | paNonInterleaved,
                                        dev->defaultHighOutputLatency, nullptr};
        auto err = Pa_OpenStream(&stream, &inparams, &outparams, Util::Singleton<Util::Config>().SampleRate,
                                 SystemTraits::channel_size, paClipOff, StreamCallback, this);
        if (err != paNoError) throw std::runtime_error("Failed to open stream");
        Pa_StartStream(stream);
        fmt::print("Opened device: {}\n", dev->name);
    }

    inline void stop() noexcept {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }

    inline algorithm_type& algo() noexcept { return algorithm; }

private:
    // Callback function for audiostream. Will call process on algorithm
    inline static int StreamCallback(const void* input, void* output, unsigned long, const PaStreamCallbackTimeInfo*,
                                     PaStreamCallbackFlags, void* object) {
        auto self = static_cast<AudioProcessor*>(object);
        auto inbuffer = static_cast<value_type* const*>(input);
        auto outbuffer = static_cast<value_type**>(output);

        for (std::size_t channel = 0; channel < SystemTraits::channels; ++channel)
            for (std::size_t sample = 0; sample < SystemTraits::channel_size; ++sample)
                self->buffers[channel][sample] = inbuffer[channel][sample];

        self->algorithm.process(self->buffers);

        for (std::size_t channel = 0; channel < SystemTraits::channels; ++channel)
            for (std::size_t sample = 0; sample < SystemTraits::channel_size; ++sample)
                outbuffer[channel][sample] = self->buffers[channel][sample];

        return paContinue;
    }

    buffer_type buffers{};
    PaStream* stream{};
    algorithm_type algorithm;
    bool running{false};
};

} // namespace Audio

#endif /* AUDIOPROCESSOR_H */
