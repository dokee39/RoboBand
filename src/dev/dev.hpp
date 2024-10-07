#pragma once

#include <cstdint>
#ifdef USE_WEBOTS
#include "io/webots.hpp"
#endif

namespace robo {
namespace dev {
template <typename IO>
class Dev {
public:
    explicit Dev(IO &io):
        io(io){
        io.unpackers.push_back([this](uint8_t *data) -> bool { return unpack(data); });
    }
    virtual ~Dev() = default;

protected:
    IO &io;

private:
    virtual bool unpack(uint8_t *data) = 0;
};

#ifdef USE_WEBOTS
template <>
class Dev<robo::io::Webots> {
public:
    explicit Dev(robo::io::Webots &webots_io):
        webots_io(webots_io) {
        webots_io.updaters.push_back([this]() { update(); });
    }
    virtual ~Dev() = default;

protected:
    robo::io::Webots &webots_io;

private:
    virtual void update() = 0;

};
#endif
}
}
