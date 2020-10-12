#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include <cstdint>

namespace Audio {

template <typename SystemTraits, typename = void>
class Passthrough {
public:
    using buffer_type = typename SystemTraits::buffer_type;
    void process(buffer_type&) {}
};

} // namespace Audio

#endif /* PASSTHROUGH_H */
