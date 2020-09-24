#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace Util {

struct Config {
    void set_logging_severity(boost::log::trivial::severity_level level) {
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= level);
    }
    double SampleRate{48000.0};
    int deviceIndex;
};

} // namespace Util

#endif /* CONFIG_H */
