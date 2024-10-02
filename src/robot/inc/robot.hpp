#pragma once

namespace robo {
class Robot {
public:
    explicit Robot();
    ~Robot() = default;

    void run();
    void stop();

private:
    void bindVirtualDev();
    void devInit();
    void bindDevIo();

};
}

