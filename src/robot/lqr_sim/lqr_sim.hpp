#ifdef USE_WEBOTS
#pragma once

#include "robot/robot.hpp"
#include "ctrl/balance_sys/balance_sys.hpp"
#include "util/util.hpp"

namespace robo {
class LqrSim: public Robot {
public:
    explicit LqrSim(const toml::table &config):
        balance_sys(util::getTable(util::getTable(config, "ctrl"), "balance_sys")) {
        webots_io = new robo::io::Webots;
    }
    ~LqrSim() override {
        delete webots_io;
    }

private:
    ctrl::BalanceSys balance_sys;
    
    void bindVirtualDev() override {}
    void devInit() override {}
    void bindDevIo() override {}
};
}

#endif

