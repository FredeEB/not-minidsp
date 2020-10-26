#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Util {

struct Config {
    std::string filterPath;
    double sampleRate{48000.0};
    int deviceIndex;
};

} // namespace Util

#endif /* CONFIG_H */
