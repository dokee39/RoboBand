#pragma once

namespace Motor {
// base class : motor
class MotorBase {
public:
    explicit MotorBase() = default;
    virtual ~MotorBase() = default;

    virtual bool ctrl() = 0;
    virtual bool feedback() = 0;

    void update(const float speed_, const float angle_) {
        speed = speed_;
        angle = angle_;
    }
    float get_speed() const {
        return speed;
    }
    float get_angle() const {
        return angle;
    }

    // for compatibility.
    // if there are additional values for control, 
    // please set them using a member function of derived class in other places like robot.update_set_task() 
    float ctrl_val = 0.0f;

private:
    MotorBase(const MotorBase&) = delete; // uncopyable
    MotorBase& operator=(const MotorBase&) = delete; // uncopyable


    float speed = 0.0f;
    float angle = 0.0f;
};

template<typename T_cfg, typename T_impl> // wait for specialization for different types of chassis
class Motor: public MotorBase {
public:
    explicit Motor(const T_cfg& config, T_impl& impl);
    ~Motor() override = default;

private:
    Motor() = delete; // must init

    T_impl& impl;
};

template<typename T_cfg> // wait for specialization for different types of chassis
class MotorImpl {
public:
    explicit MotorImpl(const T_cfg& config);
    ~MotorImpl() = default;

private:
    MotorImpl() = delete; // must init
};

} // namespace Motor
