#ifdef USE_WEBOTS

#include "io/webots.hpp"

namespace robo {
namespace io {
Webots::Webots():
    time_step(robot.step(0)),
    basic_time_step((int)robot.getBasicTimeStep()){
}
Webots::~Webots() {
    delete sync_point;
}

int Webots::step() {
    if (sync_point == nullptr) {
        sync_point = new std::barrier<>(bind_tasks_num + 1);
    }
    time_step = robot.step(basic_time_step);
    if (time_step == -1) {
        delete sync_point;
    } else {
        sync_point->arrive_and_wait();
    }
    return time_step;
}
}
}

#endif

