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

#include <bits/c++config.h>
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
        auto err = Pa_OpenDefaultStream(&stream, SystemTraits::channels, SystemTraits::channels,
                                        Util::NumberType<value_type>::value | paNonInterleaved,
                                        Util::Singleton<Util::Config>().SampleRate, SystemTraits::frame_size,
                                        StreamCallback, this);
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

        if (frameCount != SystemTraits::frame_size) throw std::runtime_error("Framecount != buffersize");

        for (std::size_t channel = 0; channel < SystemTraits::channels; ++channel)
            for (std::size_t sample = 0; sample < SystemTraits::channel_size; ++sample)
                self->buffers.at(channel).at(sample) = inbuffer[channel * SystemTraits::channel_size + sample];

        self->algorithm.process(self->buffers);

        for (std::size_t channel = 0; channel < SystemTraits::channels; ++channel)
            for (std::size_t sample = 0; sample < SystemTraits::channel_size; ++sample)
                outbuffer[channel * SystemTraits::channel_size + sample] = self->buffers.at(channel).at(sample);

        return paContinue;
    }

    buffer_type buffers{};
    PaStream* stream{};
    algorithm_type algorithm;
    bool running{false};
};

} // namespace Audio

#endif /* AUDIOPROCESSOR_H */
