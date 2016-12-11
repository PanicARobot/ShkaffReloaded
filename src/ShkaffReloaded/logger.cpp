#include "logger.h"
#include <Arduino.h>
#include <cstdint>

void sendDataToPC(float speed)
{
	// send start to tester
	Serial.write(0x03);

	int32_t encoderSpeed = (int32_t) speed;
	// send 32 bit value
	Serial.write(encoderSpeed);
	Serial.write(encoderSpeed >> 8);
	Serial.write(encoderSpeed >> 16);
	Serial.write(encoderSpeed >> 24);


	int32_t setPoint = 0;
	// send 32 bit value
	Serial.write(setPoint);
	Serial.write(setPoint >> 8);
	Serial.write(setPoint >> 16);
	Serial.write(setPoint >> 24);

	int32_t powerLevel = 0;
	// send 32 bit value
	Serial.write(powerLevel);
	Serial.write(powerLevel >> 8);
	Serial.write(powerLevel >> 16);
	Serial.write(powerLevel >> 24);

	// send end to tester
	Serial.write(0xFC);
}
