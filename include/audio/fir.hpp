#ifndef FIR_H
#define FIR_H

#include <vector>
#include <istream>
#include <numeric>

#include <audio/filter.hpp>

namespace Audio {

struct FIRTag {};

template <typename BufferType>
using FIRFilter = Filter<BufferType, FIRTag>;

template <typename BufferType, std::size_t Taps>
std::istream& operator>>(std::istream&, FIRFilter<BufferType>&);

template <typename BufferType>
class Filter<BufferType, FIRTag> {
public:
    using buffer_type = BufferType;
    using value_type = typename buffer_type::value_type;
    using delayline_type = std::vector<value_type>;

    constexpr Filter(std::size_t taps) : coeffiecients(taps), delayline(taps) {}

    void process(buffer_type& buffer) {
        for (auto bufferit = buffer.begin(); bufferit != buffer.end(); ++bufferit, ++delaylinehead) {
            if (delaylinehead == delayline.end()) delaylinehead = delayline.begin();

            *delaylinehead = *bufferit;

            *bufferit = std::transform_reduce(coeffiecients.begin(), coeffiecients.end(), delaylinehead, 0.0f);
        }
    }

private:
    delayline_type coeffiecients;
    delayline_type delayline;
    typename delayline_type::iterator delaylinehead = delayline.begin();
};

} // namespace Audio

#endif /* FIR_H */
