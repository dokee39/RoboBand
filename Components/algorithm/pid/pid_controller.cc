#include "pid_controller.hpp"

#include "user_lib_cpp.hpp"

namespace Pid {
float Pid::calc(float ref, float set) {
    err_[1] = err_[0];
    err_[0] = (set - ref);

    Pout_ = kp * err_[0];
    Iout_ += ki * err_[0];
    Dout_ = kd * (err_[0] - err_[1]);

    UserLib::abs_limit<float>(Iout_, max_iout);
    out_ = Pout_ + Iout_ + Dout_;
    UserLib::abs_limit<float>(out_, max_out);

    return out_;
}

void Pid::clean() {
    err_[0] = err_[1] = 0;
    Pout_ = Iout_ = Dout_ = 0;
    out_ = 0;
}

} // namespace Pid
