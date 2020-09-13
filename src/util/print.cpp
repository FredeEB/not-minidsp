#include <fmt/core.h>
#include <util/print.hpp>
#include <portaudiocpp/AutoSystem.hxx>
#include <portaudiocpp/Device.hxx>
#include <portaudiocpp/SystemDeviceIterator.hxx>

void print_devices() {
    auto& sys = portaudio::System::instance();

    for (auto it = sys.devicesBegin(); it != sys.devicesEnd(); ++it) {
        fmt::print("{}:\t{}\n", it->index(), it->name());
    }
}
