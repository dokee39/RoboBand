#include <iostream>
#include <fstream>
#include <string>
#include <atomic>
#include <csignal>
#include <toml++/toml.hpp>

#include "robot/robot.hpp"

std::atomic<bool> running {true};

void SigintHandler(int signum) {
    std::cout << "[INFO] Interrupt signal (" << signum << ") received!" << std::endl;
    running = false;
}

int main(int argc, char **argv) {
    std::string robot_name;
    std::string user_config_path = "./user.toml";

#ifndef USE_WEBOTS
    signal(SIGINT, SigintHandler);
#endif

    if (argc > 2) {
        std::cerr << "[ERROR] You passed the wrong command line arguments!" << std::endl;
        return 1;
    } else if (argc == 2) {
        user_config_path = argv[1];
    }

    std::ifstream tomlFile(user_config_path);
    if (!tomlFile.is_open()) {
        std::cerr << R"([Error] Could not open file ")" + user_config_path + R"("!)" << std::endl;
        return 1;
    }


    auto robot = robo::robotCreate(user_config_path);

    if (robot == nullptr) {
        return 1;
    }

    robot->run(running);

    delete robot;

    std::cout << "[INFO] Main thread exiting..." << std::endl;
}

