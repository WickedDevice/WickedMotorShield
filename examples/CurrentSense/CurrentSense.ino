#include <WickedMotorShield.h>

#define RING_BUFFER_DEPTH 16
#define NUM_MOTORS 4
Wicked_DCMotor motor1(M1);
Wicked_DCMotor motor2(M2);
Wicked_DCMotor motor3(M3);
Wicked_DCMotor motor4(M4);

Wicked_DCMotor *m[] = {&motor1, &motor2, &motor3, &motor4};
const char *   m_headings[] = {"M1", "M2", "M3", "M4"};

/*
#define NUM_MOTORS 6
Wicked_DCMotor motor1(M1);
Wicked_DCMotor motor2(M2);
Wicked_DCMotor motor3(M3);
Wicked_DCMotor motor4(M4);
Wicked_DCMotor motor5(M5);
Wicked_DCMotor motor6(M6);
Wicked_DCMotor *m[] = {&motor1, &motor2, &motor3, &motor4, &motor5, &motor6};
const char *   m_headings[] = {"M1", "M2", "M3", "M4", "M5", "M6"};
*/

uint16_t ring_buffers[NUM_MOTORS][RING_BUFFER_DEPTH];
uint8_t  ring_buffer_index;

uint32_t ringBufferAverage(uint8_t index);

void setup(void){
  Serial.begin(115200);
  Serial.print(F("Wicked Motor Shield Library version "));
  Serial.print(WickedMotorShield::version());
  Serial.println(F("- Current Sensing"));
  for(int ii = 0; ii < NUM_MOTORS; ii++){
    Serial.print(m_headings[ii]);
    Serial.print(F("\t"));
    m[ii]->setDirection(DIR_CW);
    m[ii]->setSpeed(255);
    m[ii]->setBrake(BRAKE_OFF);
  }
  Serial.println();
}

void loop(void){
  // store current samples on each motor
  for(int ii = 0; ii < NUM_MOTORS; ii++){
    ring_buffers[ii][ring_buffer_index] = m[ii]->currentSense();
  }

  // update and wrap the ring_buffer_index
  ring_buffer_index++;
  if(ring_buffer_index == RING_BUFFER_DEPTH){
    ring_buffer_index = 0;
  }
  
  // print a row of the moving average
  for(int ii = 0; ii < NUM_MOTORS; ii++){
    Serial.print(ringBufferAverage(ii));
    Serial.print(F("\t"));
  }
  Serial.println();
  delay(100);
}

uint32_t ringBufferAverage(uint8_t index){
  uint32_t sum = 0;
  for(int ii = 0; ii < RING_BUFFER_DEPTH; ii++){
    sum += ring_buffers[index][ii];
  }
  
  return (sum / RING_BUFFER_DEPTH);
}
