#pragma once

#include <cstdint>
#include <vector>

namespace UserLib {
const float PIf = 3.14159265358979323846f; /* pi */

float rad_format(const float ang);

template<typename T>
T abs(T& num)
{
    return num >= 0 ? num : -num;
}

template<typename T>
void abs_limit(T& num, const T& limit) {
    if (num > limit)
        num = limit;
    else if (num < -limit)
        num = -limit;
}

template<typename T>
void deadzone_limit(T& num, const T& deadzone) {
    if (num > -deadzone && num < deadzone)
        num = 0;
}

template<typename T>
void unpack(T& t, const void* const ptr) {
    for (size_t i = 0; i < sizeof(T); i++) {
        *((uint8_t*)&t + i) = *((uint8_t*)ptr + i);
    }
}

template<typename T>
std::vector<uint8_t> pack(const T& t) {
    std::vector<uint8_t> ans;
    for (size_t i = 0; i < sizeof(T); i++) {
        ans.push_back(*((uint8_t*)&t + i));
    }
    return ans;
}

} // namespace UserLib
