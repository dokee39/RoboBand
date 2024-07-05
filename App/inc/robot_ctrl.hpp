#pragma once

#include "chassis_base.hpp"

namespace Robot {
class RobotCtrl {
public:
    explicit RobotCtrl(Chassis::ChassisBase* chassis);
    ~RobotCtrl() = default;

    [[noreturn]] void update_set_task(void* arg);

    Chassis::ChassisBase* chassis;

private:
    RobotCtrl() = delete; // must init
    RobotCtrl(const RobotCtrl&) = delete; // uncopyable
    RobotCtrl& operator=(const RobotCtrl&) = delete; // uncopyable

    Chassis::Status& chassis_set;
};
} // namespace Robot
