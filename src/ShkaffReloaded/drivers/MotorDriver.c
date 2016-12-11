#include "MotorDriver.h"

#include <Arduino.h>

#define MOTOR_MAX_SPEED         255

#define MOTOR_LEFT_DIR_PIN      A5
#define MOTOR_LEFT_PWM_PIN      2
#define MOTOR_RIGHT_DIR_PIN     A6
#define MOTOR_RIGHT_PWM_PIN     3

void initMotors(void)
{
	// init all outputs as outputs....
	pinMode(MOTOR_LEFT_DIR_PIN, OUTPUT);
	pinMode(MOTOR_LEFT_PWM_PIN, OUTPUT);
	pinMode(MOTOR_RIGHT_DIR_PIN, OUTPUT);
	pinMode(MOTOR_RIGHT_PWM_PIN, OUTPUT);

	// set PWM to default value
	setMotors(0, 0);
}

void setMotor(int32_t speed, int dir_pin, int speed_pin)
{
	if(speed > MOTOR_MAX_SPEED) speed = MOTOR_MAX_SPEED;
	else if(speed < -MOTOR_MAX_SPEED) speed = -MOTOR_MAX_SPEED;

	digitalWrite(dir_pin, speed > 0);
	if(speed < 0) speed = -speed;
	analogWrite(speed_pin, speed);
}

void setMotors(int32_t left, int32_t right)
{
	setMotor(left, MOTOR_LEFT_DIR_PIN, MOTOR_LEFT_PWM_PIN);
	setMotor(right, MOTOR_RIGHT_DIR_PIN, MOTOR_RIGHT_PWM_PIN);
}
