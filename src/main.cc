#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#include "webots_motor.hpp"

int main(int argc, char **argv) {
    auto *robot = new webots::Robot();
    int timeStep = (int)robot->getBasicTimeStep();

    auto wm = robot->getMotor("joint_motor_1"); 

    auto m = new robo::motor::Motor("1"); 
    auto mywm = new robo::motor::WebotsMotor(wm, timeStep);
    m->bind(mywm->binder);
    
    while (robot->step(timeStep) != -1) {
        m->setTorque(20.0f);
    }
}
