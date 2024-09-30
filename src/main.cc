#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#include "io/webots/inc/webots.hpp"
#include "webots_motor.hpp"

int main(int argc, char **argv) {
    auto robot = new webots::Robot;
    int timeStep = (int)robot->getBasicTimeStep();
    auto motor = robot->getMotor("joint_motor_1"); 

    auto webots_io = new robo::io::Webots(*robot);

    auto vmotor = new robo::motor::Motor("1"); 
    auto wmotor = new robo::motor::WebotsMotor(*webots_io, *motor, timeStep);
    vmotor->bind(wmotor->binder);
    
    while (robot->step(timeStep) != -1) {
        wmotor->setTorque(20.0f);
    }
}
