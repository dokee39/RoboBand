#include <csignal>

#include "dev/inc/webots_motor.hpp"
#include "ctrl/chassis/inc/lqr_sim.hpp"

std::atomic<bool> running {true};

void SigintHandler(int signum) {
    std::cout << "[INFO] Interrupt signal (" << signum << ") received!" << std::endl;
    running = false;
}

int main(int argc, char **argv) {
    auto webots_io = new robo::io::Webots();
    auto chassis = new robo::ctrl::LqrSim();
    auto wmotor = new robo::dev::WebotsMotor(*webots_io, "joint_motor_1");
    chassis->joint_motor[0].bind(wmotor->binder);
    chassis->runner.bind(*webots_io);
    chassis->runner.run();

#ifndef USE_WEBOTS
    signal(SIGINT, SigintHandler);
#endif
    while (running && webots_io->step() != -1);

    chassis->runner.stop();
    std::cout << "[INFO] Main thread exiting..." << std::endl;
}

