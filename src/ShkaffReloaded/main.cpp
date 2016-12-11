#include "DualEncoderDriver.hpp"
#include "logger.h"
#include "MotorDriver.h"

#include <Arduino.h>

#define SERIAL_BAUD_RATE 115200

#define LEFT_ENCODER_A_PIN 0
#define LEFT_ENCODER_B_PIN 1
#define RIGHT_ENCODER_A_PIN 4
#define RIGHT_ENCODER_B_PIN 5

DualEncoder left_encoder(LEFT_ENCODER_A_PIN, LEFT_ENCODER_B_PIN);
DualEncoder right_encoder(RIGHT_ENCODER_A_PIN, RIGHT_ENCODER_B_PIN);

void setup()
{
	initMotors();

	left_encoder.init(
		[]{left_encoder.A_handler();},
		[]{left_encoder.B_handler();});
	right_encoder.init(
		[]{right_encoder.A_handler();},
		[]{right_encoder.B_handler();});

	Serial.begin(SERIAL_BAUD_RATE);
}

void loop()
{
	static uint32_t last_micros = 0;
	uint32_t current_micros = micros();

	left_encoder.update();
	right_encoder.update();

	if(current_micros - last_micros >= 50)
	{
		float left_speed = left_encoder.getSpeed();
		//float right_speed = right_encoder.getSpeed();

		sendDataToPC(left_speed);
	}
}
