#ifndef LISTDEVICES_H
#define LISTDEVICES_H

#include <fmt/core.h>
#include <portaudio.h>
#include <audio/autosystem.hpp>

void print_devices() {
    Audio::AutoSystem sys;
    fmt::print("Available Devices: \n");
    auto deviceCount = Pa_GetDeviceCount();
    for (auto i = 0; i < deviceCount; ++i) {
        auto device = Pa_GetDeviceInfo(i);
        fmt::print("{}: {}\n", i, device->name);
    }
    Pa_Terminate();
}

#endif /* LISTDEVICES_H */
