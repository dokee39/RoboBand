#include <easylogging++.h>
#include <fstream>
#include <csignal>

#include "robot_create.hpp"

INITIALIZE_EASYLOGGINGPP

std::atomic<bool> running {true};

void SigintHandler(int signum) {
    LOG(INFO) << "Interrupt signal (" << signum << ") received!";
    running = false;
}

int main(int argc, char **argv) {
#ifndef USE_WEBOTS
    signal(SIGINT, SigintHandler);
#endif
    std::string robot_name;
    std::string user_config_path = "./user.toml";

    el::Loggers::configureFromGlobal("./log/log.conf");
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    std::remove("myeasylog.log");

    LOG(DEBUG) << "Program start. ------------------------------------------";

    if (argc > 2) {
        LOG(ERROR) << "You passed the wrong command line arguments!";
        return 1;
    } else if (argc == 2) {
        user_config_path = argv[1];
    }

    std::ifstream tomlFile(user_config_path);
    if (!tomlFile.is_open()) {
        LOG(ERROR) << R"(Could not open file ")" + user_config_path + R"("!)";
        return 1;
    }

    auto robot = robo::robotCreate(user_config_path);

    if (robot == nullptr) {
        return 1;
    }

    robot->run(running);

    delete robot;

    LOG(INFO) << "Main thread exiting...";
}

