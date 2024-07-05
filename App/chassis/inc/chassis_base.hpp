#pragma once

#include <strings.h>
#include <vector>

#include "motor_base.hpp"

namespace Chassis {
struct Status {
    explicit Status(float vx, float vy, float wz): vx(vx), vy(vy), wz(wz) {}
    explicit Status() = default;

    float vx = 0.0f;
    float vy = 0.0f;
    float wz = 0.0f;
};

class ChassisBase {
public:
    explicit ChassisBase() = default;
    virtual ~ChassisBase() = default;

    virtual void init() = 0;
    [[noreturn]] virtual void task(void* arg) final;

    Status ref;
    Status set;

protected:
    std::vector<Motor::MotorBase*> motors;

private:
    ChassisBase(const ChassisBase&) = delete; // uncopyable
    ChassisBase& operator=(const ChassisBase&) = delete; // uncopyable

    virtual void update_state() = 0;
    virtual void ctrl_val_calc() = 0;
    virtual void ctrl() = 0;
};


template<typename Tcfg> // wait for specialization for different types of chassis
class Chassis: public ChassisBase {
public:
    explicit Chassis(const Tcfg& config);
    ~Chassis() override = default;

private:
    Chassis() = delete; // must init
};

} // namespace Chassis
