#pragma once

#include "io/io.hpp"
#include "serial/serial.h"

namespace robo {
namespace io {
class Serial: public IoNoKey {
public:
    // Receive data with a fixed length (buffer_size) or a specified packet tail (eol != ""), otherwise the real-time performance will be poor.
    explicit Serial(const std::string &name, const int buffer_size, const int baudrate, const int timeout_ms, const std::string &eol = "");
    ~Serial() override = default;

private:
    const std::string eol;
    serial::Serial serial;
    int baudrate;
    int timeout_ms;

    bool isConnected();
    int read(char *data) override;
    bool send(const char *message, const int len);
};
}
}
