#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include <portaudio.h>
#include <boost/circular_buffer.hpp>
#include <portaudiocpp/Stream.hxx>
#include <utility>

namespace Audio {

template <typename ValueType, std::size_t Channels = 1>
class Passthrough {
public:
    void process(boost::circular_buffer<ValueType>& buffer) {}
};

} // namespace Audio

#endif /* PASSTHROUGH_H */
