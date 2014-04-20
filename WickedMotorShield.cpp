/* Copyright (C) 201 by Victor Aprea <victor.aprea@wickeddevice.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#include "WickedMotorShield.h"

#define SERIAL_CLOCK_PIN (2)
#define SERIAL_LATCH_PIN (7)

#define OPERATION_CLEAR  (0)
#define OPERATION_SET    (1)
#define OPERATION_NONE   (2)

uint8_t WickedMotorShield::first_shift_register = 0;
uint8_t WickedMotorShield::second_shift_register = 0;
uint8_t WickedMotorShield::SERIAL_DATA_PIN = 0;
uint8_t WickedMotorShield::RCIN1_PIN = 0;
uint8_t WickedMotorShield::RCIN2_PIN = 0;
uint8_t WickedMotorShield::M1_PWM_PIN = 0;   
uint8_t WickedMotorShield::M6_PWM_PIN = 0; 
uint8_t WickedMotorShield::old_dir[6] = {0,0,0,0,0,0};


WickedMotorShield::WickedMotorShield(uint8_t serial_data_pin, uint8_t m1_pwm_pin, uint8_t m6_pwm_pin, uint8_t rcin1_pin, uint8_t rcin2_pin){
  SERIAL_DATA_PIN = serial_data_pin;  
  M1_PWM_PIN = m1_pwm_pin;
  M6_PWM_PIN = m6_pwm_pin;
  
  // intialize pins
  pinMode(SERIAL_CLOCK_PIN, OUTPUT);
  pinMode(SERIAL_LATCH_PIN, OUTPUT);
  pinMode(SERIAL_DATA_PIN, OUTPUT);  
  
  pinMode(RCIN1_PIN, INPUT);
  pinMode(RCIN2_PIN, INPUT);
  
  for(uint8_t ii = 0; ii < 6; ii++){
    old_dir[ii] = DIR_CW; // initial direction coming out of brake is clockwise
  }
  
  // load the initial values so the motors are set to a brake state initially
  load_shift_register();
}

void WickedMotorShield::load_shift_register(void){
  digitalWrite(SERIAL_LATCH_PIN, LOW);  
  shiftOut(SERIAL_DATA_PIN, SERIAL_CLOCK_PIN, LSBFIRST, second_shift_register);  
  shiftOut(SERIAL_DATA_PIN, SERIAL_CLOCK_PIN, LSBFIRST, first_shift_register);  
  digitalWrite(SERIAL_LATCH_PIN, HIGH);  
}

uint8_t WickedMotorShield::get_shift_register_value(uint8_t motor_number){
  uint8_t temp = first_shift_register;
  if(motor_number == M5 || motor_number == M6){
    temp = second_shift_register;
  }
  
  return temp;
}

void WickedMotorShield::set_shift_register_value(uint8_t motor_number, uint8_t value){
  if(motor_number == M5 || motor_number == M6){
    second_shift_register = value;
  }
  else{
    first_shift_register = value;
  }  
}


void WickedMotorShield::apply_mask(uint8_t * shift_register_value, uint8_t mask, uint8_t operation){
  switch(operation){
  case OPERATION_CLEAR:
    *shift_register_value &= ~mask;
    break;
  case OPERATION_SET:
    *shift_register_value |= mask;
    break;
  case OPERATION_NONE:
    // do nothing
    break;
  }
}

uint8_t WickedMotorShield::filter_mask(uint8_t shift_regsiter_value, uint8_t mask){
  if((shift_regsiter_value & mask) == 0){
    return 0;
  }
  
  // else
  return 1;
}

uint32_t WickedMotorShield::getRCIN(uint8_t rc_input_number, uint32_t timeout){
  
  uint8_t rc_input_pin = get_rc_input_pin(rc_input_number);
  if(rc_input_pin == 0xff){
    return 0xffffffff; //invalid RCIN number
  }
  
  if(timeout == 0){
    return pulseIn(rc_input_pin, HIGH);
  }

  //else
  return pulseIn(rc_input_pin, HIGH, timeout);
}

uint8_t WickedMotorShield::get_rc_input_pin(uint8_t rc_input_number){
  if(rc_input_number == RCIN1){
    return RCIN1_PIN;
  }
  else if(rc_input_number == RCIN2){
    return RCIN2_PIN;
  }
  
  //else
  return 0xff;
}
