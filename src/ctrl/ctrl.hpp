#pragma once

#include <string>

#include "runner/runner.hpp"

namespace robo {
namespace ctrl {
class Ctrl {
public:
    explicit Ctrl(std::string name, int cycle_ms):
        runner(name, [this]() { ctrlLoop(); }, cycle_ms) {
    }
    virtual ~Ctrl() = default;

    robo::run::Runner runner;

    virtual void ctrlLoop() = 0;

};
}
}
