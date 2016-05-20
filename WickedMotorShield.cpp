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

#include "WickedMotorShield.h"

#define SERIAL_CLOCK_PIN (2)
#define SERIAL_LATCH_PIN (7)

#define OPERATION_CLEAR  (0)
#define OPERATION_SET    (1)
#define OPERATION_NONE   (2)

uint8_t WickedMotorShield::first_shift_register = 0xff;
uint8_t WickedMotorShield::second_shift_register = 0xff;
uint8_t WickedMotorShield::SERIAL_DATA_PIN = 12;
uint8_t WickedMotorShield::RCIN1_PIN = 4;
uint8_t WickedMotorShield::RCIN2_PIN = 8;
uint8_t WickedMotorShield::M1_PWM_PIN = 11;   
uint8_t WickedMotorShield::M6_PWM_PIN = 3; 
uint8_t WickedMotorShield::old_dir[6] = {0,0,0,0,0,0};


WickedMotorShield::WickedMotorShield(uint8_t use_alternate_pins){
  
  if( use_alternate_pins == USE_ALTERNATE_PINS){
    WickedMotorShield::SERIAL_DATA_PIN = 0;
    WickedMotorShield::RCIN1_PIN = 3;
    WickedMotorShield::RCIN2_PIN = 11;
    WickedMotorShield::M1_PWM_PIN = 8;   
    WickedMotorShield::M6_PWM_PIN = 4; 
  }
  
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

// for pwm value use a value between 0 and 255
void WickedMotorShield::setSpeedM(uint8_t motor_number, uint8_t pwm_val){  
  switch(motor_number){
  case M1:
    analogWrite(M1_PWM_PIN, pwm_val);
    break;
  case M2:
    analogWrite(M2_PWM_PIN, pwm_val);  
    break;
  case M3:
    analogWrite(M3_PWM_PIN, pwm_val);  
    break;
  case M4:
    analogWrite(M4_PWM_PIN, pwm_val);  
    break;
  case M5:
    analogWrite(M5_PWM_PIN, pwm_val);  
    break;
  case M6:
    analogWrite(M6_PWM_PIN, pwm_val);  
    break;  
  }
}

void WickedMotorShield::setDirectionData(uint8_t motor_number, uint8_t direction){
  uint8_t shift_register_value = get_shift_register_value(motor_number);
  uint8_t * p_shift_register_value = &shift_register_value;
  uint8_t dir_operation   = OPERATION_NONE;
  
  if(motor_number >= 6){
    return; // invalid motor_number, go no further
  }  
  
  //TODO: is this the "correct" sense of DIR_CW / DIR_CCW
  // this explicitly becomes the old direction value
  if(direction == DIR_CW){
    dir_operation = OPERATION_SET; 
    old_dir[motor_number] = 1; 
  }
  else if(direction == DIR_CCW){
    dir_operation = OPERATION_CLEAR;
    old_dir[motor_number] = 0;    
  }

  switch(motor_number){
  case M1:
    apply_mask(p_shift_register_value, M1_DIR_MASK, dir_operation);
    break;
  case M2:
    apply_mask(p_shift_register_value, M2_DIR_MASK, dir_operation);  
    break;
  case M3:
    apply_mask(p_shift_register_value, M3_DIR_MASK, dir_operation);  
    break;
  case M4:
    apply_mask(p_shift_register_value, M4_DIR_MASK, dir_operation);  
    break;
  case M5:
    apply_mask(p_shift_register_value, M5_DIR_MASK, dir_operation);  
    break;
  case M6:
    apply_mask(p_shift_register_value, M6_DIR_MASK, dir_operation);  
    break;  
  }   
  
  set_shift_register_value(motor_number, shift_register_value);  
}

void WickedMotorShield::setBrakeData(uint8_t motor_number, uint8_t brake_type){
  uint8_t shift_register_value = get_shift_register_value(motor_number);
  uint8_t * p_shift_register_value = &shift_register_value;
  uint8_t brake_operation = OPERATION_NONE;
  uint8_t dir_operation   = OPERATION_NONE;
  
  if(motor_number >= 6){
    return; // invalid motor_number, go no further
  }
  
  // calculate the affect on the relevant shift register bits  
  if(brake_type == BRAKE_OFF){
    brake_operation = OPERATION_CLEAR;
    dir_operation = OPERATION_NONE;  
  }
  else if(brake_type == BRAKE_SOFT){
    brake_operation = OPERATION_SET;
    dir_operation = OPERATION_CLEAR;
   
  }
  else if(brake_type == BRAKE_HARD){
    brake_operation = OPERATION_SET;
    dir_operation = OPERATION_SET;          
  }
    
  // save / restore directionality
  // we already know motor_number is a safe index into old_dir because we checked earlier
  if(brake_type == BRAKE_OFF){
    // when clearing the brake, restore the old_dir value
    if(old_dir[motor_number] == 1){ 
      dir_operation = OPERATION_SET;  
    }
    else{
      dir_operation = OPERATION_CLEAR;  
    }
  }
  else if((brake_type == BRAKE_SOFT) || (brake_type == BRAKE_HARD)){
    // when applying the brake, save the old_dir value   
    old_dir[motor_number] = get_motor_directionM(motor_number);
  }
  
  switch(motor_number){
  case M1:
    apply_mask(p_shift_register_value, M1_BRAKE_MASK, brake_operation);
    apply_mask(p_shift_register_value, M1_DIR_MASK, dir_operation);
    break;
  case M2:
    apply_mask(p_shift_register_value, M2_BRAKE_MASK, brake_operation);
    apply_mask(p_shift_register_value, M2_DIR_MASK, dir_operation);  
    break;
  case M3:
    apply_mask(p_shift_register_value, M3_BRAKE_MASK, brake_operation);
    apply_mask(p_shift_register_value, M3_DIR_MASK, dir_operation);  
    break;
  case M4:
    apply_mask(p_shift_register_value, M4_BRAKE_MASK, brake_operation);
    apply_mask(p_shift_register_value, M4_DIR_MASK, dir_operation);  
    break;
  case M5:
    apply_mask(p_shift_register_value, M5_BRAKE_MASK, brake_operation);
    apply_mask(p_shift_register_value, M5_DIR_MASK, dir_operation);  
    break;
  case M6:
    apply_mask(p_shift_register_value, M6_BRAKE_MASK, brake_operation);
    apply_mask(p_shift_register_value, M6_DIR_MASK, dir_operation);  
    break;  
  }  
  
  set_shift_register_value(motor_number, shift_register_value);  
}

uint8_t WickedMotorShield::get_motor_directionM(uint8_t motor_number){
  uint8_t shift_register_value = get_shift_register_value(motor_number);
  
  switch(motor_number){
  case M1:
    return filter_mask(shift_register_value, M1_DIR_MASK);
  case M2:
    return filter_mask(shift_register_value, M2_DIR_MASK);
  case M3:
    return filter_mask(shift_register_value, M3_DIR_MASK);
  case M4:
    return filter_mask(shift_register_value, M4_DIR_MASK);
  case M5:
    return filter_mask(shift_register_value, M5_DIR_MASK);
  case M6:
    return filter_mask(shift_register_value, M6_DIR_MASK);
  }    
  
  return 0xff; // indicate error - bad motor_number argument  

}

uint8_t WickedMotorShield::version(void){
  return 1;
}



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
