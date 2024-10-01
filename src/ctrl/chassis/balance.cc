#include "ctrl/chassis/inc/balance.hpp"

namespace robo {
namespace ctrl {
void Balance::ctrl_loop() {
    joint_motor[0].setTorque(10);
}
}
}
