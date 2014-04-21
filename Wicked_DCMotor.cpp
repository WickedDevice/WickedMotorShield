/* Copyright (C) 2014 by Victor Aprea <victor.aprea@wickeddevice.com>

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

#include "Wicked_DCMotor.h"

Wicked_DCMotor::Wicked_DCMotor(uint8_t motor_number, uint8_t use_alternate_pins)
  :WickedMotorShield(use_alternate_pins){

  this->motor_number = motor_number;
}

// for direction use one of the symbols: DIR_CW, DIR_CC
void Wicked_DCMotor:: setDirection(uint8_t direction){
  setDirectionData(motor_number, direction);
  load_shift_register();
}

// for brake_type use one of the symbols: HARD, SOFT, OFF
void Wicked_DCMotor::setBrake(uint8_t brake_type){
  setBrakeData(motor_number, brake_type);
  load_shift_register();  
}

uint8_t Wicked_DCMotor::get_motor_direction(void){
  return get_motor_directionM(motor_number);
}

uint16_t Wicked_DCMotor::currentSense(void){
  switch(motor_number){
  case M1:
    return analogRead(A0);
  case M2:
    return analogRead(A2);
  case M3:
    return analogRead(A1);
  case M4:
    return analogRead(A3);
  case M5:
    return analogRead(A4);
  case M6:
    return analogRead(A5);
  }    
  
  return 0xffff; // indicate error - bad motor_number argument
}

void Wicked_DCMotor::setSpeed(uint8_t pwm_val){
  setSpeedM(motor_number, pwm_val);
}
