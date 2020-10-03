#ifndef ROOMCORRECTION_H
#define ROOMCORRECTION_H

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <ostream>
#include <utility>
#include <vector>

#include <audio/biquad.hpp>
#include <audio/fir.hpp>
#include <audio/parallel.hpp>
#include <util/repeat_type.hpp>

namespace Audio {

template <typename SystemTraits, typename Tag = void>
class RoomCorrection;

template <typename SystemTraits>
class RoomCorrection<SystemTraits, BiquadTag> {
public:
    using buffer_type = typename SystemTraits::buffer_type;
    using filter_type = Biquad<typename SystemTraits::channel_type>;
    using process_type = typename Util::repeat_type<filter_type, SystemTraits::channels, Parallel>::type;

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
    using buffer_type = typename SystemTraits::buffer_type;
    using filter_type = FIRFilter<buffer_type>;

    constexpr RoomCorrection(std::size_t taps) : filter(taps) {}

    void process(buffer_type& buffer) { filter.process(buffer); }

    void loadFilterFromFile(std::string const& path) {
        std::ifstream file(path);
        file >> filter;
    }

private:
    filter_type filter{};
};

} // namespace Audio

#endif /* ROOMCORRECTION_H */
