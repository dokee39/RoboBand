#pragma once

namespace Pid {
struct PidConfig {
    float kp;
    float ki;
    float kd;

    float max_iout;
    float max_out;
};

class Pid: public PidConfig {
public:
    explicit Pid(const PidConfig& config): PidConfig(config) {}
    ~Pid() = default;

    float calc(float ref, float set);
    void clean();

private:
    float err_[2] = {}; // 误差项 0最新 1上一次 2上上次

    float Pout_ = 0.f;
    float Iout_ = 0.f;
    float Dout_ = 0.f;

    float out_ = 0.f;
};

} // namespace Pid
