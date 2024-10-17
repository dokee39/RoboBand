#pragma once

#include <cstring>
#include <functional>
#include <vector>
#include <string>
#include <thread>
#include <map>
#include <easylogging++.h>

namespace robo {
namespace io {
class Io {
public:
    explicit Io(const std::string &name, const int buffer_size):
        name(name),
        buffer_size(buffer_size),
        buffer(new char[buffer_size]) {
    }
    virtual ~Io() {
        delete[] buffer;
        if (thread && thread->joinable()) {
            thread->join();
        }
    }

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
            LOG(INFO) << "[IO<" + name + ">] Stopped!";
        }
    }

protected:
    const std::string name;
    const int buffer_size;
    char *buffer;
    std::atomic<bool> running {false};

    virtual void thread_func() = 0;

private:
    std::thread *thread {nullptr};

};

class IoKey: public Io {
public:
    explicit IoKey(const std::string &name, const int buffer_size):
        Io(name, buffer_size) {
    }
    ~IoKey() override = default;

    std::map<int, std::function<void (const char *, const int len)>> unpackers;

    virtual int read(int &key, char *data) = 0;

private:
    std::atomic<bool> running;

    void thread_func() override {
        while (running) {
            int len, key;
            len = read(key, buffer);
            if (len <= 0) {
                LOG(WARNING) << "[IO<" + name + ">] Read Error!";
            } else {
                auto it = unpackers.find(key);
                if (it == unpackers.end()) {
                    LOG(WARNING) << "[IO<" + name + ">] Unbound key read: " << key << ".";
                } else {
                    it->second(buffer, len);
                }
            }
        }
    }
};

class IoNoKey: public Io {
public:
    explicit IoNoKey(const std::string &name, const int buffer_size):
        Io(name, buffer_size) {
    }
    ~IoNoKey() override = default;

    std::vector<std::function<bool (const char *, const int len)>> unpackers;

    virtual int read(char *data) = 0;

private:
    std::atomic<bool> running;

    void thread_func() override {
        while (running) {
            memset(buffer, 0, buffer_size);
            int len = read(buffer);
            if (len <= 0) {
                LOG(WARNING) << "[IO<" + name + ">] Read Error!";
            } else {
                bool is_unpacked = false;
                for (auto &unpacker: unpackers) {
                    is_unpacked = unpacker(buffer, len);
                    if (is_unpacked) {
                        break;
                    }
                }
                if (!is_unpacked) {
                    LOG(WARNING) << "[IO<" + name + ">] The data read does not have a matching unpacker.";
                }
            }
        }
    }
};
}
}

