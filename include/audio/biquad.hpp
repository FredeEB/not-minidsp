#ifndef BIQUAD_H
#define BIQUAD_H

#include <array>
#include <istream>
#include <utility>

#include <audio/filter.hpp>

namespace Audio {

struct BiquadTag {};

template <typename BufferType>
using Biquad = Filter<BufferType, BiquadTag>;

template <typename BufferType>
std::istream& operator>>(std::istream&, Biquad<BufferType>&);
template <typename BufferType>
std::ostream& operator<<(std::ostream&, Biquad<BufferType> const&);

template <typename BufferType>
class Filter<BufferType, BiquadTag> {
public:
    using buffer_type = BufferType;
    using value_type = typename buffer_type::value_type;

    constexpr Filter() = default;
    constexpr Filter(value_type ca1, value_type ca2, value_type cb0, value_type cb1, value_type cb2)
            : a1{ca1}, a2{ca2}, b0{cb0}, b1{cb1}, b2{cb2} {}

    void process(buffer_type& buffer) {
        for (std::size_t i = 0; i < buffer.size(); ++i) {
            xn2 = xn1;
            xn1 = xn0;
            xn0 = buffer[i];

            buffer[i] = b0 * xn0 + b1 * xn1 + b2 * xn2 - a1 * yn1 - a2 * yn2;

            yn2 = yn1;
            yn1 = buffer[i];
        }
    }

    friend std::istream& operator>><buffer_type>(std::istream&, Filter&);
    friend std::ostream& operator<<<buffer_type>(std::ostream&, Filter const&);

private:
    // TODO: Should coefficients always be doubles, or should they be scaled at construction time if integral?
    value_type a1{}, a2{}, b0{}, b1{}, b2{};
    value_type yn1{}, yn2{}, xn0{}, xn1{}, xn2{};
};

template <typename BufferType>
std::istream& operator>>(std::istream& is, Biquad<BufferType>& c) {
    return is >> c.a1 >> c.a2 >> c.b0 >> c.b1 >> c.b2;
}
template <typename BufferType>
std::ostream& operator<<(std::ostream& os, Biquad<BufferType> const& filter) {
    os << "Biquad: "
       << "\na1: " << filter.a1 << "\na2: " << filter.a2 << "\nb0: " << filter.b0 << "\nb1: " << filter.b1
       << "\nb2: " << filter.b2 << '\n';
    return os;
}

} // namespace Audio

#endif /* BIQUAD_H */
