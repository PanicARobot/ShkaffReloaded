#ifndef __PID_HPP
#define __PID_HPP

class PidController {
	private:
		const float kP, kI, kD;
		float last_error, integral;
		float integral_flank;

	public:
		PidController(float, float, float, float);

		void zero();
		float sample(float, float, float);
};

#endif
