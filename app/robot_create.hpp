#pragma once

#include "src/robot/robot.hpp"

namespace robo {
Robot *robotCreate(const std::string &user_config_path);
}
