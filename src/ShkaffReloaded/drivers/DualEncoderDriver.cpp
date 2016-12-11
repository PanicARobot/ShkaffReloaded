#include "DualEncoderDriver.hpp"

#include <Arduino.h>

#define MICROS_PER_SECOND    1000000
#define WHEEL_REDUCTION      29.86f
#define WHEEL_DIAMETER       32

#define RESET_TIMEOUT_MULTIPLIER 3

#define WHEEL_PERIMETER      (WHEEL_DIAMETER * M_PI)
#define IMPULSES_PER_WHEEL   (IMPULSES_PER_ROUND * WHEEL_REDUCTION)
#define DISTANCE_PER_IMPULSE (MICROS_PER_SECOND * WHEEL_PERIMETER / IMPULSES_PER_WHEEL)

DualEncoder::DualEncoder(int a_pin, int b_pin) :
	A_PIN(a_pin), B_PIN(b_pin),
	last_micros(0),
	impulse_counter(0),
	impulse_deltas_sum(0),
	impulse_deltas_index(0),
	direction(0)
	{
		for(auto& x : impulse_deltas) x = 0;
	}

void DualEncoder::init(void (*a_handler)(), void (*b_handler)())
{
	pinMode(A_PIN, INPUT);
	pinMode(B_PIN, INPUT);

	attachInterrupt(digitalPinToInterrupt(A_PIN), a_handler, CHANGE);
	attachInterrupt(digitalPinToInterrupt(B_PIN), b_handler, CHANGE);
}

void DualEncoder::update(int8_t dir)
{
	uint32_t current_micros = micros();

	dir = dir * 2 - 1; // 1 is forward
	if(dir != direction)
	{
		impulse_counter = 1;
		direction = dir;

		impulse_deltas_sum = 0;
	}
	else
	{
		if(impulse_deltas_index + 1 == QUEUE_SIZE)
			impulse_deltas_index = 0;
		else ++impulse_deltas_index;

		if(impulse_counter <= QUEUE_SIZE)
			++impulse_counter;
		else impulse_deltas_sum -= impulse_deltas[impulse_deltas_index];

		impulse_deltas[impulse_deltas_index] = current_micros - last_micros;
		impulse_deltas_sum += impulse_deltas[impulse_deltas_index];

		// speed = DISTANCE_PER_IMPULSE * (impulse_counter - 1) / impulse_deltas_sum;
	}

	last_micros = current_micros;
}

void DualEncoder::A_handler() {
	update(digitalRead(A_PIN) == digitalRead(B_PIN));
}

void DualEncoder::B_handler() {
	update(digitalRead(A_PIN) != digitalRead(B_PIN));
}

float DualEncoder::getSpeed()
{
	uint32_t current_delta = micros() - last_micros;
	uint32_t time = current_delta < impulse_deltas[impulse_deltas_index] ?
		impulse_deltas[impulse_deltas_index] : current_delta;

	return direction * DISTANCE_PER_IMPULSE / time;
}
