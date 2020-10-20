#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Util {

struct Config {
    std::string FilterPath;
    std::string Algorithm;
    double SampleRate{48000.0};
    int DeviceIndex;
};

} // namespace Util

#endif /* CONFIG_H */
