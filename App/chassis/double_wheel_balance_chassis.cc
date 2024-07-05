#include "double_wheel_balance_chassis.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "config.hpp"
#include "double_wheel_balance_chassis_config.hpp"

using namespace Config::Chassis::DoubleWheelChassis;
using Here = Config::Chassis::DoubleWheelBalanceChassisConfig; 

static void imu_task(void* arg) {
    static_cast<Dev::Imu*>(arg)->task(nullptr);
}

static void chassis_task(void* arg) {
    static_cast<Chassis::Chassis<Here>*>(arg)->task(nullptr);
}

namespace Chassis {
Chassis<Here>::Chassis(const Here& config):
    lqr(config.LQR),
    pid_tilt(config.PID_TILT_ANGLE),
    pid_vx(config.PID_VX),
    pid_yaw(config.PID_YAW),
    yaw(imu.angle[2]),
    tilt_angle(imu.angle[1]),
    tilt_speed(imu.gyro[1]) {
    TaskHandle_t xCreatedImuTask = *(new TaskHandle_t);
    TaskHandle_t xCreatedChassisTask = *(new TaskHandle_t);
    
    auto motor_imp_left = new Motor::MotorImpl(config.MOTOR_IMPL_LEFT);
    auto motor_imp_right = new Motor::MotorImpl(config.MOTOR_IMPL_RIGHT);
    Motor::MotorBase* motor_left = new Motor::Motor(config.MOTOR, *motor_imp_left);
    Motor::MotorBase* motor_right = new Motor::Motor(config.MOTOR, *motor_imp_right);

    motors.emplace_back(motor_left);
    motors.emplace_back(motor_right);

    pid_motors.emplace_back(config.PID_MOTOR_LEFT);
    pid_motors.emplace_back(config.PID_MOTOR_RIGHT);

    xTaskCreate(
        imu_task,
        "imu task",
        configMINIMAL_STACK_SIZE * 8,
        static_cast<void*>(&this->imu),
        (tskIDLE_PRIORITY + Config::Priority::IMU),
        &xCreatedImuTask
    );
    xTaskCreate(
        chassis_task,
        "chassis task",
        configMINIMAL_STACK_SIZE * 8,
        static_cast<void*>(this),
        (tskIDLE_PRIORITY + Config::Priority::CHASSIS),
        &xCreatedChassisTask
    );
}

void Chassis<Here>::init() {}

void Chassis<Here>::update_state() {
    motors[0]->feedback();
    motors[1]->feedback();

    ref.vx = (motors[0]->get_speed() + motors[1]->get_speed()) * WHEEL_DIAMETER / 2.0f;
    ref.vy = 0.0f;
    ref.wz = imu.gyro[2];

    displacement += ref.vx * (float)Config::Time::CHASSIS_CYCLE / 1000.0f;
    if (set.vx != 0)
        displacement = 0.0f;

    yaw_set += set.wz * (float)Config::Time::CHASSIS_CYCLE / 1000.0f;
}

template<>
void Chassis<Here>::ctrl_val_calc_<CTRL_LQR>() {
    float z_set[] = { 0.0f, set.vx, TILT_ANGLE_SET, 0.0f, yaw_set, set.wz };
    float z_ref[] = { displacement, ref.vx, tilt_angle, tilt_speed, yaw, ref.wz };

    lqr.update_set(z_set);
    lqr.update_ref(z_ref);

    lqr.calc();
    
    motors[0]->ctrl_val = lqr.get_u(0);
    motors[1]->ctrl_val = lqr.get_u(1);
#warning "TODO : add scale?"
}

template<>
void Chassis<Here>::ctrl_val_calc_<CTRL_PID>() {
    float tilt_angle_set(0.0f);
    float speed_set_tilt(0.0f);
    float speed_set_yaw(0.0f);
    if (UserLib::abs(tilt_angle) < TILT_ANGLE_LAZY) {
        speed_set_yaw = pid_yaw.calc(yaw, yaw_set);
        tilt_angle_set = pid_vx.calc(ref.vx, set.vx) + TILT_ANGLE_SET;
        speed_set_tilt = -pid_tilt.calc(tilt_angle, tilt_angle_set);
        motors[0]->ctrl_val = pid_motors[0].calc(motors[0]->get_speed(), speed_set_tilt + speed_set_yaw);
        motors[1]->ctrl_val = pid_motors[1].calc(motors[1]->get_speed(), speed_set_tilt - speed_set_yaw);
    } else {
        motors[0]->ctrl_val = MOTOR_CTRL_VAL_MAX * tilt_angle / UserLib::abs((tilt_angle));
        motors[1]->ctrl_val = MOTOR_CTRL_VAL_MAX * tilt_angle / UserLib::abs((tilt_angle));
    }
}

void Chassis<Here>::ctrl_val_calc() {
    ctrl_val_calc_<CTRL_METHOD>();
}

void Chassis<Here>::ctrl() {
    motors[0]->ctrl();
    motors[1]->ctrl();
}
} // namespace Chassis
