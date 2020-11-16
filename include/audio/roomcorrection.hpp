#ifndef ROOMCORRECTION_H
#define ROOMCORRECTION_H

#include <algorithm>
#include <fstream>
#include <iterator>
#include <utility>
#include <vector>

#include <audio/biquad.hpp>
#include <audio/fir.hpp>
#include <audio/parallel.hpp>
#include <util/repeat_type.hpp>
#include <audio/convolution.hpp>

namespace Audio {

template <typename Tag, typename SystemTraits>
class RoomCorrection;

template <typename SystemTraits>
class RoomCorrection<SystemTraits, BiquadTag> {
public:
    using system_traits = SystemTraits;
    using buffer_type = typename system_traits::buffer_type;
    using filter_type = Biquad<typename system_traits::channel_type>;
    using process_type = typename Util::repeat_type<filter_type, system_traits::channels, Parallel>::Type;

    void process(buffer_type& buffer) {
        for (auto& filter : filters) filter.process(buffer);
    }

    void registerFilter(filter_type&& filter) { filters.push_back(std::forward<decltype(filter)>(filter)); }
    void clearFilters() { filters.clear(); }

    void loadFiltersFromFile(std::string const& path) {
        std::ifstream file(path);
        if (!file.is_open()) throw std::runtime_error("Couldn't open file");
        std::istream_iterator<filter_type> begin(file), end;
        std::transform(begin, end, std::back_inserter(filters),
                       [](auto const& filter) { return process_type(filter); });
    }

    void printfilters(std::ostream& where) {
        std::transform(filters.begin(), filters.end(), std::ostream_iterator<filter_type>(where, "\n"),
                       [](auto& filter) { return filter.template get<0>(); });
    }

private:
    std::vector<process_type> filters{};
};

template <typename SystemTraits>
class RoomCorrection<SystemTraits, FIRTag> {
public:
    using system_traits = SystemTraits;
    using value_type = typename system_traits::value_type;
    using buffer_type = typename system_traits::buffer_type;
    using filter_type = FIRFilter<typename system_traits::channel_type>;
    using process_type = typename Util::repeat_type<filter_type, system_traits::channels, Parallel>::Type;

    void process(buffer_type& buffer) { filter.process(buffer); }

    void loadFiltersFromFile(std::string const& path) {
        std::ifstream file(path);
        std::istream_iterator<value_type> begin(file), end;
        std::vector<value_type> coeffs;
        std::copy(begin, end, std::back_inserter(coeffs));
        filter = process_type(coeffs);
    }

private:
    process_type filter{};
};

template <typename SystemTraits>
class RoomCorrection<SystemTraits, ConvolutionTag> {
public:
    using system_traits = SystemTraits;
    using value_type = typename system_traits::value_type;
    using buffer_type = typename system_traits::buffer_type;
    using filter_type = ConvolutionFilter<4096, typename system_traits::channel_type>;
    using process_type = typename Util::repeat_type<filter_type, system_traits::channels, Parallel>::Type;

    void process(buffer_type& buffer) { filter.process(buffer); }

    void loadFiltersFromFile(std::string const& path) {
        std::ifstream file(path);
        std::istream_iterator<value_type> begin(file), end;
        typename filter_type::complex_buffer_type coeffs{};
        std::copy(begin, end, coeffs.begin());
        filter = process_type(coeffs);
    }

private:
    process_type filter{};
};

} // namespace Audio

#endif /* ROOMCORRECTION_H */
