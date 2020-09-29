#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include <cstdint>

namespace Audio {

template <typename ValueType, std::size_t Channels = 1>
class Passthrough {
public:
    void process(boost::circular_buffer<ValueType>& buffer) {}
};

} // namespace Audio

#endif /* PASSTHROUGH_H */
