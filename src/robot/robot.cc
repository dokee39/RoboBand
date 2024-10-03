#include <iostream>
#include <string_view>
#include <toml++/toml.hpp>

#include "robot/robot.hpp"
#include "robot/lqr_sim/lqr_sim.hpp"

namespace robo {
Robot *robotCreate(std::string user_config_path) {
    toml::table user_config;

    try {
        user_config = toml::parse_file(user_config_path);
    } catch (const toml::parse_error &err) {
        std::cerr << R"([ERROR] Parsing your config ")" + user_config_path + R"(" failed!)" << std::endl;
        std::cerr << err << std::endl;
        return nullptr;
    }

    if (!user_config.contains("robot")) {
        std::cerr << R"([ERROR] Your config ")" + user_config_path + R"(" does not contain a key named "robot"!)" << std::endl;
        return nullptr;
    }

    auto robot_name = user_config["robot"].value_or(std::string_view("err"));
    
    if (robot_name == "default") {
        return nullptr;
#ifdef USE_WEBOTS
    } else if (robot_name == "lqr_sim") {
        return new LqrSim(user_config_path);
#endif
    } else if (robot_name == "err") {
        std::cerr << R"([ERROR] The value paired with the key "robot" in your config ")" + user_config_path + R"(" is not a string!)" << std::endl;
        return nullptr;
    } else {
        std::cerr << R"([ERROR] The "robot" you specified in the config ")" + user_config_path + R"(" has no corresponding implementation!)" << std::endl;
        return nullptr;
    }
}

void Robot::run(std::atomic<bool>& running) {
    for (auto ctrl: ctrls) {
        ctrl->runner.run();
    }

#ifdef USE_WEBOTS
    while (running && webots_io.step() != -1);
#else
    while (running);
#endif

    for (auto ctrl: ctrls) {
        ctrl->runner.stop();
    }
}
}
