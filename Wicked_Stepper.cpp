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

#include "Wicked_Stepper.h"

Wicked_Stepper::Wicked_Stepper(uint16_t number_of_steps, uint8_t m1, uint8_t m2, uint8_t use_alternate_pins)
  :WickedMotorShield(use_alternate_pins){

  this->step_number = 0;                      // which step the motor is on
  this->speed = 0;                            // the motor speed, in revolutions per minute
  this->direction = 0;                        // motor direction
  this->last_step_time = 0;                   // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor

  this->m1 = m1;
  this->m2 = m2;

  setSpeedM(m1, 255);
  setSpeedM(m2, 255);
  setDirectionData(m1, DIR_CW);
  setDirectionData(m2, DIR_CW);
  setBrakeData(m1, BRAKE_OFF);
  setBrakeData(m2, BRAKE_OFF);
  load_shift_register();
}

void Wicked_Stepper::setSpeed(uint32_t speed){
  this->step_delay = 60L * 1000L / this->number_of_steps / speed;
}

void Wicked_Stepper::step(int16_t number_of_steps){
  int steps_left = abs(number_of_steps);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (number_of_steps > 0) {this->direction = 1;}
  if (number_of_steps < 0) {this->direction = 0;}


  // decrement the number of steps, moving one step each time:
  while(steps_left > 0) {
  // move only if the appropriate delay has passed:
  if (millis() - this->last_step_time >= this->step_delay) {
      // get the timeStamp of when you stepped:
      this->last_step_time = millis();
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1) {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, 2, or 3:
      stepMotor(this->step_number % 4);
    }
  }
}

//TODO: convert the code below into analogous shift register loads
void Wicked_Stepper::stepMotor(int this_step){
  switch (this_step) {
  case 0:    // 1010
    setDirectionData(m1, DIR_CW);
    setDirectionData(m2, DIR_CCW);
    break;
  case 1:    // 0110
    setDirectionData(m1, DIR_CCW);
    setDirectionData(m2, DIR_CCW);
    break;
  case 2:    // 0101
    setDirectionData(m1, DIR_CCW);
    setDirectionData(m2, DIR_CW);
    break;
  case 3:    // 1001
    setDirectionData(m1, DIR_CW);
    setDirectionData(m2, DIR_CW);
    break;
  }

  load_shift_register();
}
