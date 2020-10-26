#ifndef LISTDEVICES_H
#define LISTDEVICES_H

#include <portaudio.h>
#include <audio/autosystem.hpp>
#include <iostream>

void print_devices() {
    Audio::AutoSystem sys;
    std::cout << "Available Devices:\n";
    auto deviceCount = Pa_GetDeviceCount();
    for (auto i = 0; i < deviceCount; ++i) {
        auto device = Pa_GetDeviceInfo(i);
        std::cout << i << ": " << device->name << '\n';
    }
    Pa_Terminate();
}

#endif /* LISTDEVICES_H */
