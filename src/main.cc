#include <csignal>
#include <iostream>
#include <atomic>
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#include "io/webots/inc/webots.hpp"
#include "webots_motor.hpp"
#include "ctrl/chassis/inc/balance.hpp"

std::atomic<bool> running {true};

void Interrupt(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received!" << std::endl;
    running = false;
}

int main(int argc, char **argv) {
    std::signal(SIGINT, Interrupt);

    auto webots_io = new robo::io::Webots();

    auto motor = webots_io->robot.getMotor("joint_motor_1"); 

    auto chassis = new robo::ctrl::Balance();

    auto wmotor = new robo::motor::WebotsMotor(*webots_io, *motor);
    chassis->joint_motor[0].bind(wmotor->binder);

    chassis->runner.bind(*webots_io);
    chassis->runner.run(1);

    while (running && webots_io->Step() != -1) {
    }

    chassis->runner.stop();

    std::cout << "Main thread exiting..." << std::endl;
}
