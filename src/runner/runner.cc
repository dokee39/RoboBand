#include <iostream>

#include "runner/inc/runner.hpp"

namespace robo {
namespace run {
#ifdef USE_WEBOTS
void Runner::run() {
    if (webots_io == nullptr) {
        std::cerr << "[ERROR] [Runner<" + name + ">] You haven't bound robo::io::webots before running!" << std::endl;
        return;
    }

    running = true;
    if (thread != nullptr) {
        std::cerr << "[ERROR] [Runner<" + name + ">] Repeated run a Runner!" << std::endl;
    } else {
        thread = new std::thread([this]() {
            int time_step_set = webots_io->time_step;
            while (running) {
                time_step_set += cycle_ms;
                if (webots_io != nullptr) {
                    if (webots_io->sync_point != nullptr) 
                        webots_io->sync_point->arrive_and_wait();
                }
                if (time_step_set > webots_io->time_step) {
                    task();
                }
            }
        });
    }
}

void Runner::stop() {
    running = false;
    if (thread != nullptr) {
        thread->join();
        delete thread;
        webots_io->bind_tasks_num --;
        std::clog << "[INFO] [Runner<" + name + ">] Stopped!" << std::endl;
    }
}

void Runner::bind(robo::io::Webots &webots_io_) {
    webots_io = &webots_io_;
    webots_io->bind_tasks_num ++;
}

#else
void Runner::run() {
    running = true;
    if (thread == nullptr) {
        std::cerr << "[ERROR] [Runner<" + name + ">] Repeated run a Runner!" << std::endl;
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
        std::clog << "[INFO] [Runner<" + name + ">] Stopped!" << std::endl;
    }
}

#endif
}
}

