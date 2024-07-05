#include "user_lib_cpp.hpp"

#include <cmath>

namespace UserLib {
float rad_format(const float ang) {
    float ans = fmodf(ang + PIf, PIf * 2.f);
    return (ans < 0.f) ? ans + PIf : ans - PIf;
}

} // namespace UserLib
