#include "DualEncoderDriver.hpp"

#include <Arduino.h>

#include <cmath>

#define MICROS_PER_SECOND 1000000
#define WHEEL_REDUCTION   29.86
#define WHEEL_DIAMETER       32
#define WHEEL_PERIMETER      (WHEEL_DIAMETER * M_PI)
#define IMPULSES_PER_WHEEL   (IMPULSES_PER_ROUND * WHEEL_REDUCTION)
#define DISTANCE_PER_IMPULSE (MICROS_PER_SECOND * WHEEL_PERIMETER / IMPULSES_PER_WHEEL)

DualEncoder::DualEncoder(int a_pin, int b_pin)
	: A_PIN(a_pin), B_PIN(b_pin),
	peak_started(false),
	last_signal_change(0),
	signal_change_delta(0)
{
}

void DualEncoder::init(void (*handler)())
{
	pinMode(A_PIN, INPUT);
	pinMode(B_PIN, INPUT);

	attachInterrupt(digitalPinToInterrupt(A_PIN), handler, CHANGE);
	attachInterrupt(digitalPinToInterrupt(B_PIN), handler, CHANGE);
}

void DualEncoder::handler()
{
	uint32_t us = micros();
	signal_change_delta = us - last_signal_change;
	last_signal_change = us;
}

float DualEncoder::getSpeed()
{
	uint32_t current_delta = micros() - last_signal_change;

	uint32_t speed = current_delta < signal_change_delta ? signal_change_delta : current_delta;

	return 1000000.0f / (speed * 12);
}
