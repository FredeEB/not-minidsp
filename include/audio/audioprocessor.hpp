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

#include <util/numbertype.hpp>
#include <util/repeat_type.hpp>
#include <util/clihandler.hpp>
#include <audio/parallel.hpp>
#include <audio/processortraits.hpp>

#include <portaudio.h>
#include <iostream>

namespace Audio {
template <typename Algorithm>
class AudioProcessor {
public:
    using system_traits = typename Algorithm::system_traits;
    using value_type = typename system_traits::value_type;
    using channel_type = typename system_traits::channel_type;
    using buffer_type = typename system_traits::buffer_type;

    AudioProcessor(Algorithm& algo) : algorithm{algo} {}
    ~AudioProcessor() { stop(); }

    inline void run() {
        auto const deviceIndex = Util::getConfig().deviceIndex;
        PaDeviceInfo const* dev = Pa_GetDeviceInfo(deviceIndex);
        if (dev->maxInputChannels < system_traits::channels)
            throw std::runtime_error("Device doesn't have anough input channels for requested configuration");
        if (dev->maxOutputChannels < system_traits::channels)
            throw std::runtime_error("Device doesn't have anough output channels for requested configuration");

        PaStreamParameters inparams = {deviceIndex, system_traits::channels,
                                       Util::NumberType<value_type>::value | paNonInterleaved,
                                       dev->defaultHighInputLatency, nullptr};
        PaStreamParameters outparams = {deviceIndex, system_traits::channels,
                                        Util::NumberType<value_type>::value | paNonInterleaved,
                                        dev->defaultHighOutputLatency, nullptr};
        auto err = Pa_OpenStream(&stream, &inparams, &outparams, Util::getConfig().sampleRate,
                                 system_traits::channel_size, paClipOff, StreamCallback, this);
        if (err != paNoError) throw std::runtime_error("Failed to open stream");
        Pa_StartStream(stream);
        std::cout << "Opened device: " << dev->name << std::endl;
    }

    inline void stop() noexcept {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }

private:
    // Callback function for audiostream. Will call process on algorithm
    inline static int StreamCallback(const void* input, void* output, unsigned long, const PaStreamCallbackTimeInfo*,
                                     PaStreamCallbackFlags, void* object) {
        auto self = static_cast<AudioProcessor*>(object);
        auto inbuffer = static_cast<value_type* const*>(input);
        auto outbuffer = static_cast<value_type**>(output);

        for (std::size_t channel = 0; channel < system_traits::channels; ++channel)
            for (std::size_t sample = 0; sample < system_traits::channel_size; ++sample)
                self->buffers[channel][sample] = inbuffer[channel][sample];

        self->algorithm.process(self->buffers);

        for (std::size_t channel = 0; channel < system_traits::channels; ++channel)
            for (std::size_t sample = 0; sample < system_traits::channel_size; ++sample)
                outbuffer[channel][sample] = self->buffers[channel][sample];

        return paContinue;
    }

    buffer_type buffers{};
    PaStream* stream{};
    Algorithm& algorithm;
    bool running{false};
};

} // namespace Audio

#endif /* AUDIOPROCESSOR_H */
