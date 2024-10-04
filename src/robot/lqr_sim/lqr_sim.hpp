#ifdef USE_WEBOTS
#pragma once

#include "robot/robot.hpp"
#include "ctrl/balance_sys/balance_sys.hpp"

namespace robo {
class LqrSim: public Robot {
public:
    explicit LqrSim(const toml::table &config):
        balance_sys(*config["ctrl"]["balance_sys"].as_table()) {
    }
    ~LqrSim() override = default;

private:
    ctrl::BalanceSys balance_sys;
    
    void bindVirtualDev() override {}
    void devInit() override {}
    void bindDevIo() override {}
};
}

#endif

