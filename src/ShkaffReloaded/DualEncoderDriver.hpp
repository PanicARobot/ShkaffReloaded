#ifndef __DUAL_ENCODER_DRIVER_H
#define __DUAL_ENCODER_DRIVER_H

#include <cstdint>

#define IMPULSES_PER_ROUND   12
#define QUEUE_SIZE IMPULSES_PER_ROUND

class DualEncoder {
	private:
		const int A_PIN;
		const int B_PIN;

		volatile bool peak_started;
		volatile uint32_t last_signal_change;
		volatile uint32_t signal_change_delta;

		void update(int8_t);

	public:
		DualEncoder(int, int);

		void init(void (*)());

		void handler();

		float getSpeed();
};

#endif // __DUAL_ENCODER_DRIVER_H
