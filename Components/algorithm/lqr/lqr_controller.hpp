#pragma once

#include "matrix.h"
#include "user_lib_cpp.hpp"

namespace Lqr {
template<int _rows, int _cols>
void abs_limit(Matrixf<_rows, _cols>& M, Matrixf<_rows, _cols>& Mmax) {
    for (int row = 0; row < _rows; row++)
        for (int col = 0; col < _cols; col++)
            UserLib::abs_limit<float>(M[row][col], const_cast<float &>(Mmax[row][col]));
}

template<int dz, int du>
struct LqrConfig {
    const Matrixf<du, dz> K;
    const Matrixf<1, du> Umax;
};

template<int dz, int du>
class Lqr {
public:
    explicit Lqr(const LqrConfig<dz, du>& config):
        set((float[dz]) { 0.0f }),
        ref((float[dz]) { 0.0f }),
        Z((float[dz]) { 0.0f }),
        U((float[du]) { 0.0f }),
        K(config.K),
        Umax(config.Umax) {}
    ~Lqr() = default;

    void calc() {
#warning "TODO : use delta control"
        Z = ref - set;
        U = (-K * Z).trans();
        abs_limit<1, du>(U, Umax);
    }
    int dim_z() {
        return dz;
    }
    int dim_u() {
        return du;
    }
    void update_set(float data[dz])
    {
        set.update(data);
    }
    void update_ref(float data[dz])
    {
        ref.update(data);
    }
    float get_u(const int i)
    {
        return U[0][i];
    }

private:
    Lqr() = delete; // must init
    Lqr(const Lqr&) = delete; // uncopyable
    Lqr& operator=(const Lqr&) = delete; // uncopyable

#warning "Matrixf cannot be a const"
    Matrixf<dz, 1> set;
    Matrixf<dz, 1> ref;
    Matrixf<dz, 1> Z;
    Matrixf<1, du> U;
    Matrixf<du, dz> K;
    Matrixf<1, du> Umax;
};
} // namespace Lqr
