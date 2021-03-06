#include "drivers/DualEncoderDriver.hpp"
#include "drivers/MotorDriver.h"
#include "controlled_movement.h"

#include "logger.h"

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
	static uint32_t last_millis = 0;
	uint32_t current_millis = millis();

	if(current_millis - last_millis >= 100)
	{
		float left_speed = left_encoder.getSpeed();
		float right_speed = right_encoder.getSpeed();

		handleControlledMovement(left_speed, right_speed, 1000.0f / (current_millis - last_millis));

		sendDataToPC(left_speed);

		last_millis = current_millis;
	}
}
