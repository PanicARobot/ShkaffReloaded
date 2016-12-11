#define     MOTOR_ENCODER_A             2
#define     MOTOR_ENCODER_B             3

#define     LED_PIN                     13

#define     LOOP_TIME_10ms              5
#define     LOOP_TIME_100ms             100
#define     LOOP_TIME_1s                1000

#define     ENCODER_INTEGRAL_FORCE      1

// encoder needed variables
uint32_t encoderSpeed, setPoint = 1000, powerLevel, encoderLastSignalChange = 0, encoderLastTimeBetweenPeaks = 0, bufferForEncoder;

// main tasks time handling
uint32_t LastFire10ms, LastFire100ms, LastFire1s, currentMillis;

// integration filter for encoders
float sumOfEncoderSpeed = 0.0;

void setup()
{
    Serial.begin(115200);

    pinMode(MOTOR_ENCODER_A , INPUT);
    pinMode(MOTOR_ENCODER_B , INPUT);

    pinMode(LED_PIN         , OUTPUT);

    attachInterrupt(0, handleEncoderPinChange, CHANGE);
    attachInterrupt(1, handleEncoderPinChange, CHANGE);
}

void loop()
{
    currentMillis = millis();
    // 10 ms loop
    if(currentMillis - LastFire10ms > LOOP_TIME_10ms)
    {
        // update motor speed
        bufferForEncoder = getMotorSpeed();

        if (bufferForEncoder < 1500 && bufferForEncoder != 0 && bufferForEncoder != 82) // only when data is not over motor specs
        {
            encoderSpeed = bufferForEncoder;
        }

        // integrate the encoder output to reach smoother values
        sumOfEncoderSpeed -= (sumOfEncoderSpeed - ((float)encoderSpeed)) * ENCODER_INTEGRAL_FORCE;

        // load the speed in the output variable
        encoderSpeed = sumOfEncoderSpeed;

        // run 10 ms tasks
        sendDataToPC();

        // update lat fire
        LastFire10ms = currentMillis;
    }
}

uint32_t getMotorSpeed()
{
    // response buffer
    uint32_t responseVariable;

    // in case the current delay is lower than the last, we need to return the last as the current is still in progress
    if( micros() - encoderLastSignalChange < encoderLastTimeBetweenPeaks)
    {
        // load the response in the buffer
        responseVariable = encoderLastTimeBetweenPeaks;
    }
    else
    {
        // load the current time delay as the speed response
        responseVariable = micros() - encoderLastSignalChange;
    }

    // convert the encoder value to speed in RPM
    responseVariable = 1000000 / (responseVariable * 12);

    // response to the caller
    return responseVariable;
}

void handleEncoderPinChange()
{
    // static flag for peak detection
    static uint8_t peakStarted = 0;

    // in case the peak ended we need to update the last time between peaks
    if (1 == peakStarted)
    {
        // load the new peak to peak time in the variable
        encoderLastTimeBetweenPeaks = micros() - encoderLastSignalChange;

        // reset peak started flag
        peakStarted = 0;
    }
    else
    {
        // set peak started flag
        peakStarted = 1;
    }

    // update last signal detected
    encoderLastSignalChange = micros();
}

void sendDataToPC()
{
  // send start to tester
  Serial.write(0x03);

  // send 32 bit value
  Serial.write(encoderSpeed);
  Serial.write(encoderSpeed >> 8);
  Serial.write(encoderSpeed >> 16);
  Serial.write(encoderSpeed >> 24);

  // send 32 bit value
  Serial.write(setPoint);
  Serial.write(setPoint >> 8);
  Serial.write(setPoint >> 16);
  Serial.write(setPoint >> 24);

  // send 32 bit value
  Serial.write(powerLevel);
  Serial.write(powerLevel >> 8);
  Serial.write(powerLevel >> 16);
  Serial.write(powerLevel >> 24);
  
  // send end to tester
  Serial.write(0xFC);
}
