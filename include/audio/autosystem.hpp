#ifndef AUTOSYSTEM_H
#define AUTOSYSTEM_H

#include <portaudio.h>
#include <cstdio>
#include <stdexcept>

namespace Audio {

class AutoSystem {
public:
    AutoSystem() {
        freopen("/dev/null", "w", stderr);
        auto err = Pa_Initialize();
        freopen("/dev/tty", "w", stderr);
        if (err != paNoError) throw std::runtime_error("Failed to initialize Portaudio");
    }
    AutoSystem(AutoSystem const &) = delete;
    AutoSystem operator=(AutoSystem const &) = delete;
    AutoSystem(AutoSystem &&) = delete;
    AutoSystem operator=(AutoSystem &&) = delete;
    ~AutoSystem() noexcept { Pa_Terminate(); }
};

} // namespace Audio

#endif /* AUTOSYSTEM_H */
