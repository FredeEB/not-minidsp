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
std::istream& operator>>(std::istream&, FIRFilter<BufferType>&);

template <typename BufferType>
class Filter<BufferType, FIRTag> {
public:
    using buffer_type = BufferType;
    using value_type = typename buffer_type::value_type;
    using delayline_type = std::vector<value_type>;

    constexpr Filter(std::size_t taps) : coefficients(taps), delayline(taps) {}

    void process(buffer_type& buffer) {
        std::transform(buffer.begin(), buffer.end(), buffer.begin(),
                       [&](auto sample) { return calculateSample(sample); });
    }

    friend std::istream& operator>><buffer_type>(std::istream&, Filter&);

private:
    inline value_type calculateSample(value_type value) {
        if (delaylinehead == std::prev(delayline.begin())) delaylinehead = std::prev(delayline.end());

        *delaylinehead = value;

        value_type res = std::transform_reduce(std::execution::par, coefficients.begin(), coefficients.end(),
                                               circular_iterator(delayline, delaylinehead), 0.0f);

        --delaylinehead;

        return res;
    }

    delayline_type coefficients;
    delayline_type delayline;
    typename delayline_type::iterator delaylinehead = delayline.begin();
};

template <typename BufferType>
std::istream& operator>>(std::istream& is, FIRFilter<BufferType>& filter) {
    std::istream_iterator<typename BufferType::value_type> begin(is), end;
    std::copy(begin, end, std::back_inserter(filter.coefficients));
    return is;
}

} // namespace Audio

#endif /* FIR_H */
