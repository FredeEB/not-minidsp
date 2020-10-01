#ifndef ROOMCORRECTION_H
#define ROOMCORRECTION_H

#include <cstdint>
#include <fstream>
#include <iterator>
#include <utility>
#include <vector>

#include <audio/biquad.hpp>

namespace Audio {

template <typename BufferType, std::size_t Channels, typename Tag = void>
class RoomCorrection;

template <typename BufferType, std::size_t Channels>
class RoomCorrection<BufferType, Channels, BiquadTag> {
public:
    using value_type = typename BufferType::value_type;
    using buffer_type = BufferType;
    using filter_type = Biquad<buffer_type>;

    void process(buffer_type& buffer) {
        for (auto& filter : filters) filter.process(buffer);
    }

    void registerFilter(filter_type&& filter) { filters.push_back(std::forward<decltype(filter)>(filter)); }
    void clearFilters() { filters.clear(); }

    void loadFiltersFromFile(std::string const& path) {
        std::ifstream file(path);
        std::istream_iterator<filter_type> begin(file), end;
        std::copy(begin, end, std::back_inserter(filters));
    }

private:
    std::vector<filter_type> filters;
};

} // namespace Audio

#endif /* ROOMCORRECTION_H */
