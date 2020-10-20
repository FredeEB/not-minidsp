#ifndef FIR_H
#define FIR_H

#include <algorithm>
#include <iterator>
#include <vector>
#include <istream>
#include <numeric>
#include <execution>

#include <audio/filter.hpp>
#include <audio/circularIterator.hpp>

namespace Audio {

struct FIRTag {};

template <typename BufferType>
using FIRFilter = Filter<BufferType, FIRTag>;

template <typename BufferType>
class Filter<BufferType, FIRTag> {
public:
    using buffer_type = BufferType;
    using value_type = typename buffer_type::value_type;
    using delayline_type = std::vector<value_type>;

    constexpr Filter() = default;
    constexpr Filter(delayline_type coeffs) : coefficients(std::move(coeffs)), delayline(coefficients.size()) {}

    inline void process(buffer_type& buffer) noexcept {
        std::transform(buffer.begin(), buffer.end(), buffer.begin(),
                       [&](auto sample) { return calculateSample(sample); });
    }

private:
    inline value_type calculateSample(value_type value) noexcept {
        *delaylinehead = value;

        value_type res = std::transform_reduce(std::execution::par, coefficients.begin(), coefficients.end(),
                                               circular_iterator(delayline, delaylinehead), 0.0f);

        if (delaylinehead == delayline.begin()) delaylinehead = delayline.end();
        --delaylinehead;

        return res;
    }

    delayline_type coefficients;
    delayline_type delayline;
    typename delayline_type::iterator delaylinehead = delayline.begin();
};

} // namespace Audio

#endif /* FIR_H */
