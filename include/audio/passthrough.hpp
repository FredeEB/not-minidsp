#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include <cstdint>

namespace Audio {

template <typename BufferType, std::size_t Channels, typename = void>
class Passthrough {
public:
    using buffer_type = BufferType;
    void process(buffer_type&) {}
};

} // namespace Audio

#endif /* PASSTHROUGH_H */
