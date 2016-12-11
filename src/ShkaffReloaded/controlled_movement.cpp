#include "controlled_movement.h"

#include "drivers/MotorDriver.h"
#include "utility/pid.hpp"

#include <cstdint>

static float left_target_speed = 0;
static float right_target_speed = 0;

void setMotorsSpeed(float left, float right)
{
	left_target_speed = left;
	right_target_speed = right;
}

// TODO: adjust PID controllers
#define MOTOR_KP 1
#define MOTOR_KI 1
#define MOTOR_KD 1

static PidController left_pid(MOTOR_KP, MOTOR_KI, MOTOR_KI, 255 / MOTOR_KI);
static PidController right_pid(MOTOR_KP, MOTOR_KI, MOTOR_KI, 255 / MOTOR_KI);

void handleControlledMovement(float left_speed, float right_speed, float delta_time)
{
	float left_power = left_pid.sample(left_target_speed, left_speed, delta_time);
	float right_power = right_pid.sample(right_target_speed, right_speed, delta_time);

	setMotors(left_power, right_power);
}
