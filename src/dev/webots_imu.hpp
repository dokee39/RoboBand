#pragma once
#ifdef USE_WEBOTS

#include <string>

#include "virtual/imu.hpp"
#include "io/webots.hpp"

namespace robo {
namespace dev {
class WebotsImu {
public:
    explicit WebotsImu(robo::io::Webots &webots_io, const std::string &inertial_name, const std::string &gyro_name);
    ~WebotsImu() = default;

    robo::vir::ImuBinder binder {
        .angle = this->angle,
        .speed = this->speed
    };

    void update();

private:
    robo::io::Webots &webots_io;
    webots::InertialUnit &inertial;
    webots::Gyro &gyro;

    float angle[3];
    float speed[3];

};
}
}

#endif

