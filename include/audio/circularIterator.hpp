#ifndef CIRCULARITERATOR_H
#define CIRCULARITERATOR_H

#include <iterator>

namespace Audio {
template <typename BufferType, typename Iter>
class circular_iterator : public std::iterator<std::forward_iterator_tag, BufferType> {
public:
    using value_type = typename BufferType::value_type;
    using perimeter_iterator = typename BufferType::iterator;
    using buffer_iterator = Iter;

    circular_iterator(BufferType& buffer, buffer_iterator offset)
            : begin(buffer.begin()), end(buffer.end()), p(offset) {}

    circular_iterator& operator++() {
        if (p == end) p = begin;
        ++p;
        return *this;
    }

    circular_iterator operator++(int) {
        if (p == end) p = begin;
        circular_iterator tmp(p);
        ++p;
        return tmp;
    }

    value_type& operator*() { return *p; }
    value_type& operator->() { return *p; }

private:
    perimeter_iterator begin, end;
    Iter p;
};
} // namespace Audio

#endif /* CIRCULARITERATOR_H */
