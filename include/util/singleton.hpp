#ifndef SINGLETON_H
#define SINGLETON_H

namespace Util {

template <typename T>
T& Singleton() {
    static T instance;
    return instance;
}

} // namespace Util

#endif /* SINGLETON_H */
