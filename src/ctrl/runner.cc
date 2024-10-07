#include <easylogging++.h>

#include "ctrl/runner.hpp"

namespace robo {
namespace run {
#ifdef USE_WEBOTS
void Runner::run() {
    if (webots_io == nullptr) {
        LOG(ERROR) << "[Runner<" + name + ">] You haven't bound robo::io::webots before running!";
        return;
    }
    if (thread != nullptr) {
        LOG(ERROR) << "[Runner<" + name + ">] Repeated run a Runner!";
        return;
    }

    running = true;
    thread = new std::thread([this]() { thread_func(); });
    LOG(INFO) << "[Runner<" + name + ">] Start running...";
}

void Runner::stop() {
    running = false;
    if (thread != nullptr) {
        thread->join();
        delete thread;
        LOG(INFO) << "[Runner<" + name + ">] Stopped!";
    }
}

void Runner::bind(robo::io::Webots &webots_io_) {
    webots_io = &webots_io_;
    webots_io->bind_runner_num++;
}

void Runner::thread_func() {
    int time_step_set = webots_io->time_step;
    while (running) {
        time_step_set += cycle_ms;
        if (webots_io != nullptr) {
            if (webots_io->sync_point != nullptr) {
                webots_io->sync_point->arrive_and_wait();
            }
            if (time_step_set >= webots_io->time_step) {
                task();
            }
        }
    }
}

#else
void Runner::run() {
    running = true;
    if (thread == nullptr) {
        LOG(ERROR) << "[Runner<" + name + ">] Repeated run a Runner!";
    } else {
        thread = new std::thread([this]() {
            while (running) {
                task();
            }
        });
    }
}

void Runner::stop() {
    running = false;
    if (thread != nullptr) {
        thread->join();
        delete thread;
        LOG(INFO) << "[Runner<" + name + ">] Stopped!";
    }
}

#endif
} // namespace run
} // namespace robo
