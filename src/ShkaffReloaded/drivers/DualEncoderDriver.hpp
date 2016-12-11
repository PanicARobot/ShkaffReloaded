#ifndef __DUAL_ENCODER_DRIVER_H
#define __DUAL_ENCODER_DRIVER_H

#include <cstdint>
#include <cmath>

#define IMPULSES_PER_ROUND   12
#define QUEUE_SIZE           12

class DualEncoder {
	private:
		const int A_PIN;
		const int B_PIN;

		volatile uint32_t last_micros;
		volatile int8_t impulse_counter;
		volatile uint32_t impulse_deltas[QUEUE_SIZE];
		volatile uint32_t impulse_deltas_sum;
		volatile uint8_t impulse_deltas_index;
		volatile int8_t direction;

		void update(int8_t);

	public:
		DualEncoder(int, int);

		void init(void (*)(), void (*)());

		void A_handler();
		void B_handler();

		float getSpeed(); // mm / s
};

#endif // __DUAL_ENCODER_DRIVER_H
