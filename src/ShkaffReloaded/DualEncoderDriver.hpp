#ifndef __DUAL_ENCODER_DRIVER_H
#define __DUAL_ENCODER_DRIVER_H

#include <cstdint>
#include <cmath>

#define IMPULSES_PER_ROUND   12
#define WHEEL_REDUCTION   29.86
#define WHEEL_DIAMETER       32


class DualEncoder {
	private:
		uint32_t last_micros;
		int8_t impulse_counter;

		uint32_t impulse_deltas[IMPULSES_PER_ROUND];
		uint32_t impulse_deltas_sum;
		uint8_t impulse_deltas_index;

		float speed;

		const int A_PIN;
		const int B_PIN;

		void update(int8_t);

	public:
		DualEncoder(int, int);

		void init(void (*)(), void (*)());
		void update();

		void A_handler();
		void B_handler();

		inline float getSpeed() { return speed; }; // mm / s
};

#endif // __DUAL_ENCODER_DRIVER_H