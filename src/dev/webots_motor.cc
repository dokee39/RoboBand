#include "dev/inc/webots_motor.hpp"

namespace robo {
namespace dev {
WebotsMotor::WebotsMotor(robo::io::Webots &webots_io, webots::Motor&motor): 
    webots_io(webots_io),
    motor(motor),
    encoder(*motor.getPositionSensor()) {
    encoder.enable(webots_io.robot.getBasicTimeStep());
}

void WebotsMotor::update(const int time_step) {
    int time;
    webots_io.encoderGetValue(encoder, angle, time);
    speed = (angle - angle_last) / (time - time_last);
    time_last = time;
    angle_last = angle;
}
}
}
