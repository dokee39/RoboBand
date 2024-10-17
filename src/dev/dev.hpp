#pragma once

#include <type_traits>
#include <easylogging++.h>

#include "io/io.hpp"
#ifdef USE_WEBOTS
#include "io/webots.hpp"
#endif

namespace robo {
namespace dev {
template <typename IO, typename Enable = void>
class Dev;

template <typename IO>
class Dev<IO, typename std::enable_if<std::is_base_of<robo::io::IoKey, IO>::value>::type> {
public:
    explicit Dev(const std::string &name, IO &io, const int io_key):
        io(io),
        io_key(io_key) {
        auto it = io.unpackers.find();
        if (it != io.unpackers.end()) {
            LOG(ERROR) << "[Dev<" + name + R"(>] You used duplicate key ")" << io_key << R"(" when binging to IO ")" << io.name << R"("!)";
        } else {
            io.unpackers.emplace(io_key, [this](const char *data, const int len) -> bool { return unpack(data, len); });
        }
    }
    virtual ~Dev() = default;

    const std::string name;

protected:
    IO &io;
    const int io_key;

private:
    virtual bool unpack(const char *data, const int len) = 0;
};

template <typename IO>
class Dev<IO, typename std::enable_if<std::is_base_of<robo::io::IoNoKey, IO>::value>::type> {
public:
    explicit Dev(const std::string &name, IO &io):
        io(io) {
        io.unpackers.push_back([this](const char *data, const int len) -> bool { return unpack(data, len); });
    }
    virtual ~Dev() = default;

    const std::string name;

protected:
    IO &io;

private:
    virtual bool unpack(const char *data, const int len) = 0;
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
