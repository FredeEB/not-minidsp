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

namespace Audio {

template <typename BufferType, std::size_t Channels, typename Tag = void>
class RoomCorrection;

template <typename BufferType, std::size_t Channels>
class RoomCorrection<BufferType, Channels, BiquadTag> {
public:
    using value_type = typename BufferType::value_type;
    using buffer_type = BufferType;
    using filter_type = Biquad<buffer_type>;
    using process_type = typename repeat_type<filter_type, Channels, Parallel>::type;

    void process(buffer_type& buffer) {
        for (auto& filter : filters) {
            filter.process(buffer);
        }
    }

    void registerFilter(filter_type&& filter) { filters.push_back(std::forward<decltype(filter)>(filter)); }
    void clearFilters() { filters.clear(); }

    // void loadFiltersFromFile(std::string const& path) {
    //     std::ifstream file(path);
    //     std::istream_iterator<filter_type> begin(file), end;
    //     std::copy(begin, end, std::back_inserter(filters));
    // }

    void printfilters(std::ostream& where) {
        std::copy(filters.begin(), filters.end(), std::ostream_iterator<filter_type>(where, "\n"));
    }

private:
    std::vector<process_type> filters;
};

template <typename BufferType, std::size_t Channels>
class RoomCorrection<BufferType, Channels, FIRTag> {
public:
    using value_type = typename BufferType::value_type;
    using buffer_type = BufferType;
    using filter_type = FIRFilter<buffer_type>;

    constexpr RoomCorrection(std::size_t taps) : filter(taps) {}

    void process(buffer_type& buffer) { filter.process(buffer); }

    void loadFiltersFromFile(std::string const& path) {
        std::ifstream file(path);
        file >> filter;
    }

private:
    filter_type filter;
};

} // namespace Audio

#endif /* ROOMCORRECTION_H */
