#ifdef USE_WEBOTS
#pragma once

#include "robot/robot.hpp"

namespace robo {
class LqrSim: public Robot {
public:
    explicit LqrSim(std::string user_config_path) {}
    ~LqrSim() override = default;

private:
    void parse_config() override {}
    void bindVirtualDev() override {}
    void devInit() override {}
    void bindDevIo() override {}
};
}

#endif

