#include "dev/inc/webots_motor.hpp"

namespace robo {
namespace dev {
WebotsMotor::WebotsMotor(robo::io::Webots &webots_io, const std::string& motor_name):
    webots_io(webots_io),
    motor(*webots_io.robot.getMotor(motor_name)),
    encoder(*motor.getPositionSensor()) {
    encoder.enable(webots_io.robot.getBasicTimeStep());
}

void WebotsMotor::update(const int time_step) {
    int time;
    float angle_tmp;
    webots_io.encoderGetValue(encoder, angle_tmp, time);
    angle = angle_tmp - angle_offset;
    speed = (angle - angle_last) / (time - time_last);
    time_last = time;
    angle_last = angle;
}
}
}
