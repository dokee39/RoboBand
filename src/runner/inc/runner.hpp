#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <pthread.h>

#ifdef USE_WEBOTS
#include "io/webots/inc/webots.hpp"
#endif

namespace robo {
namespace run {
class Runner {
public:
    explicit Runner(std::string name, std::function<void ()> task):
        name(name),
        task(task) {
    }
    ~Runner() {
        delete thread;
    }

    void run(int cycle_ms);
    void stop();
#ifdef USE_WEBOTS
    void bind(robo::io::Webots &webots_io_);
#endif

private:
    std::string name;
    std::atomic<bool> running {false};
    std::function<void ()> task;
    std::thread *thread {nullptr};
#ifdef USE_WEBOTS
    robo::io::Webots *webots_io;
#endif

};
}
}
