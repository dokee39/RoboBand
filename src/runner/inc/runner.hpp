#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <functional>
#include <pthread.h>
#include <chrono>

#ifdef USE_WEBOTS
#include "io/webots/inc/webots.hpp"
#endif

namespace robo {
namespace run {
#ifdef USE_WEBOTS
class Runner {
public:
    explicit Runner(std::string name, std::function<void ()> task):
        name(name),
        task(task) {
    }
    ~Runner() {
        delete thread;
    }

    void bind(robo::io::Webots &webots_io_) {
        webots_io = &webots_io_;
        webots_io->bind_tasks_num ++;
    }

    void run(int cycle_ms) {
        if (webots_io == nullptr) {
            std::cerr << "[ERROR] task <" + name + ">: You haven't bound robo::io::webots before running!" << std::endl;
            return;
        }

        running = true;
        if (thread != nullptr) {
            std::cerr << "[ERROR] task <" + name + ">: Repeated run a Runner!" << std::endl;
        } else {
            thread = new std::thread([this, cycle_ms]() {
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

    void stop() {
        running = false;
        if (thread != nullptr) {
            thread->join();
            delete thread;
            webots_io->bind_tasks_num --;
            std::cout << "[INFO] task <" + name + ">: Stopped!" << std::endl;
        }
    }

private:
    std::string name;
    std::atomic<bool> running {false};
    std::function<void ()> task;
    std::thread *thread {nullptr};
    robo::io::Webots *webots_io;

};

#else
class Runner {
public:
    explicit Runner(std::string name, std::function<void ()> task):
        name(name),
        task(task) {
    }
    explicit Runner(std::function<void ()> task):
        task(task) {
    }
    ~Runner() {
        delete thread;
    }

    void run() {
        running = true;
        if (thread == nullptr) {
            std::cerr << "[ERROR] task <" + name + ">: Repeated run a Runner!" << std::endl;
        } else {
            thread = new std::thread([this]() {
                while (running) {
                    task();
                }
            });
        }
    }

    void stop() {
        running = false;
        if (thread != nullptr) {
            thread->join();
            delete thread;
            std::cout << "[INFO] task <" + name + ">: Stopped!" << std::endl;
        }
    }

    void delay(int ms);

private:
    std::string name {"default name"};
    std::atomic<bool> running {false};
    std::function<void ()> task;
    std::thread *thread {nullptr};

};

#endif
}
}
