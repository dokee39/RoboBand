#include <iostream>
#include <string_view>

#include "robot/robot.hpp"
#include "robot/lqr_sim/lqr_sim.hpp"
#include "util/util.hpp"

namespace robo {
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

Robot *robotCreate(std::string user_config_path) {
    toml::table config;
    toml::table user_config;

    try {
        user_config = toml::parse_file(user_config_path);
    } catch (const toml::parse_error &err) {
        LOG(ERROR) << R"(Parsing your config ")" + user_config_path + R"(" failed!)";
        LOG(ERROR) << err;
        return nullptr;
    }

    if (!user_config.contains("robot")) {
        LOG(ERROR) << R"(Your config ")" + user_config_path + R"(" does not contain a key named "robot"!)";
        return nullptr;
    }

    auto robot_name = user_config["robot"].value_or(std::string_view("err"));
    
    if (robot_name == "default") {
        return nullptr;
#ifdef USE_WEBOTS
    } else if (robot_name == "lqr_sim") {
        config = toml::parse_file("./src/robot/lqr_sim/default.toml");
        util::merge_config(user_config, config);
        return new LqrSim(config);
#endif
    } else if (robot_name == "err") {
        LOG(ERROR) << R"(The value paired with the key "robot" in your config ")" + user_config_path + R"(" is not a string!)";
        return nullptr;
    } else {
        LOG(ERROR) << R"(The "robot" you specified in the config ")" + user_config_path + R"(" has no corresponding implementation!)";
        return nullptr;
    }
}
}

