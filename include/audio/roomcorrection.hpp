#ifndef ROOMCORRECTION_H
#define ROOMCORRECTION_H

#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <istream>
#include <iterator>
#include <utility>
#include <vector>

#include <bits/c++config.h>
#include <portaudio.h>
#include <portaudiocpp/Stream.hxx>

#include <audio/biquad.hpp>

namespace Audio {

template <typename T, std::size_t FramesPerBuffer = 64, std::size_t Channels = 2>
class RoomCorrection {
public:
    using value_type = T;
    using Filter_type = Biquad<value_type, FramesPerBuffer, Channels>;

    void process(std::array<value_type, FramesPerBuffer>& buffer) {
        for (auto& filter : filters) filter.process(buffer);
    }

    void registerFilter(Filter_type&& filter) { filters.push_back(std::forward<decltype(filter)>(filter)); }
    void unregisterFilter(std::size_t index) { filters.erase(index); }

    void loadFiltersFromFile(std::string const& path) {
        std::ifstream file(path);
        std::istream_iterator<Filter_type> begin(file), end;
        std::copy(begin, end, std::back_inserter(filters));
    }

private:
    std::vector<Filter_type> filters;
};

} // namespace Audio

#endif /* ROOMCORRECTION_H */
