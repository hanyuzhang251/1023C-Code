#ifndef UTIL_H
#define UTIL_H

namespace chisel {
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
}

#endif //UTIL_H