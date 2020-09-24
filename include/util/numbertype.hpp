#ifndef NUMBERTYPE_H
#define NUMBERTYPE_H

#include <portaudiocpp/SampleDataFormat.hxx>
#include <type_traits>

namespace Util {

template <typename T>
struct NumberType {
    static_assert(std::is_same_v<struct Undefined, T>, "Unsupported numbertype");
};

template <>
struct NumberType<char> {
    static portaudio::SampleDataFormat const value{portaudio::INT8};
};

template <>
struct NumberType<short> {
    static portaudio::SampleDataFormat const value{portaudio::INT16};
};

template <>
struct NumberType<int> {
    static portaudio::SampleDataFormat const value{portaudio::INT24};
};

template <>
struct NumberType<float> {
    static portaudio::SampleDataFormat const value{portaudio::FLOAT32};
};

} // namespace Util

#endif /* NUMBERTYPE_H */
