#pragma once

#include <string>

#include "runner/runner.hpp"

namespace robo {
namespace ctrl {
class Ctrl {
public:
    explicit Ctrl(const std::string &name, const int cycle_ms):
        runner(name, [this]() { ctrlLoop(); }, cycle_ms) {
    }
    virtual ~Ctrl() = default;

    robo::run::Runner runner;

    virtual void ctrlLoop() = 0;

};
}
}
