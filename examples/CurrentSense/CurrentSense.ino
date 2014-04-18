#include <WickedMotor.h>
WickedMotor motors;

#define NUM_MOTORS 4
#define RING_BUFFER_DEPTH 16
uint8_t  m[NUM_MOTORS] = {M1, M2, M3, M4};
char *   m_headings[NUM_MOTORS] = {"M1", "M2", "M3", "M4"};
uint16_t ring_buffers[NUM_MOTORS][RING_BUFFER_DEPTH];
uint8_t  ring_buffer_index;

uint32_t ringBufferAverage(uint8_t index);

void setup(void){
  Serial.begin(115200);
  Serial.println(F("Wicked Motor Shield - Current Sensing"));
  for(int ii = 0; ii < NUM_MOTORS; ii++){
    Serial.print(m_headings[ii]);
    Serial.print(F("\t"));
  }
  Serial.printnl();
}

void loop(void){
  // store current samples on each motor
  for(int ii = 0; ii < NUM_MOTORS; ii++){
    ring_buffers[ii][ring_buffer_index] = motors.currentSense(m[ii]);
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
