#include "ctrl/chassis/inc/balance.hpp"

namespace robo {
namespace ctrl {
Balance::Balance():
    runner("balance chassis", [this]() { ctrlLoop(); }) {
}
void Balance::ctrlLoop() {
    joint_motor[0].setTorque(10);
}
}
}
