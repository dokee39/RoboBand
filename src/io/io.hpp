#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <thread>
#include <easylogging++.h>

namespace robo {
namespace io {
class Io {
public:
    explicit Io(const std::string &name, const int buffer_size):
        name(name),
        buffer_size(buffer_size) {
        data = new uint8_t[buffer_size];
    }
    virtual ~Io() {
        delete[] data;
    }

    std::vector<std::function<bool (uint8_t *)>> unpackers;

    void run() {
        if (thread == nullptr) {
            LOG(ERROR) << "[IO<" + name + ">] Repeated run an IO!";
            return;
        }
        running = true;
        thread = new std::thread([this]() { thread_func(); });
    }

    void stop() {
        running = false;
        if (thread != nullptr) {
            thread->join();
            delete thread;
            LOG(INFO) << "[IO<" + name + ">] Stopped!";
        }
    }

    virtual int read(uint8_t *data) = 0;

protected:
    const std::string name;
    const int buffer_size;
    uint8_t *data;

private:
    std::thread *thread {nullptr};
    std::atomic<bool> running {false};

    void thread_func() {
        while (running) {
            if (read(data) <= 0) {
                LOG(WARNING) << "[IO<" + name + ">] Read Error!";
            } else {
                for (auto &unpacker: unpackers) {
                    if (unpacker(data) == true) {
                        break;
                    }
                }
            }
        }
    }

};
}
}
