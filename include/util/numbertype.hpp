#ifndef NUMBERTYPE_H
#define NUMBERTYPE_H

#include <portaudio.h>
#include <type_traits>

namespace Util {

template <typename T>
struct NumberType {
    static_assert(std::is_same_v<struct Undefined, T>, "Unsupported numbertype");
};

template <>
struct NumberType<char> {
    static PaSampleFormat const value{paInt8};
};

template <>
struct NumberType<short> {
    static PaSampleFormat const value{paInt16};
};

template <>
struct NumberType<int> {
    static PaSampleFormat const value{paInt24};
};

template <>
struct NumberType<float> {
    static PaSampleFormat const value{paFloat32};
};

} // namespace Util

#endif /* NUMBERTYPE_H */
