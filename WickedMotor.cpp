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

#include "WickedMotor.h"

#define SERIAL_CLOCK_PIN (2)
#define SERIAL_LATCH_PIN (7)

#define OPERATION_CLEAR  (0)
#define OPERATION_SET    (1)
#define OPERATION_NONE   (2)

WickedMotor::WickedMotor(uint8_t serial_data_pin, uint8_t m1_pwm_pin, uint8_t m6_pwm_pin, uint8_t rcin1_pin, uint8_t rcin2_pin){
  first_shift_register = 0xff;
  second_shift_register = 0xff;
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

// for motor_number use one of the symbols: M1, M2, M3, M4, M5, M6
// for pwm value use a value between 0 and 255
void WickedMotor::setSpeed(uint8_t motor_number, uint8_t pwm_val){  
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

// for motor_number use one of the symbols: M1, M2, M3, M4, M5, M6
// for direction use one of the symbols: DIR_CW, DIR_CC
void WickedMotor:: setDirection(uint8_t motor_number, uint8_t direction){
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
  load_shift_register();
}

// for motor_number use one of the symbols: M1, M2, M3, M4, M5, M6
// for brake_type use one of the symbols: HARD, SOFT, OFF
void WickedMotor::setBrake(uint8_t motor_number, uint8_t brake_type){
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
    old_dir[motor_number] = get_motor_direction(motor_number);
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
  load_shift_register();  
}

void WickedMotor::load_shift_register(void){
  digitalWrite(SERIAL_LATCH_PIN, LOW);  
  shiftOut(SERIAL_DATA_PIN, SERIAL_CLOCK_PIN, LSBFIRST, second_shift_register);  
  shiftOut(SERIAL_DATA_PIN, SERIAL_CLOCK_PIN, LSBFIRST, first_shift_register);  
  digitalWrite(SERIAL_LATCH_PIN, HIGH);  
}

uint8_t WickedMotor::get_shift_register_value(uint8_t motor_number){
  uint8_t temp = first_shift_register;
  if(motor_number == M5 || motor_number == M6){
    temp = second_shift_register;
  }
  
  return temp;
}

void WickedMotor::set_shift_register_value(uint8_t motor_number, uint8_t value){
  if(motor_number == M5 || motor_number == M6){
    second_shift_register = value;
  }
  else{
    first_shift_register = value;
  }  
}


void WickedMotor::apply_mask(uint8_t * shift_register_value, uint8_t mask, uint8_t operation){
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

uint16_t WickedMotor::currentSense(uint8_t motor_number){
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

uint8_t WickedMotor::get_motor_direction(uint8_t motor_number){
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

uint8_t WickedMotor::filter_mask(uint8_t shift_regsiter_value, uint8_t mask){
  if((shift_regsiter_value & mask) == 0){
    return 0;
  }
  
  // else
  return 1;
}

uint32_t WickedMotor::getRCIN(uint8_t rc_input_number, uint32_t timeout){
  
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

uint8_t WickedMotor::get_rc_input_pin(uint8_t rc_input_number){
  if(rc_input_number == RCIN1){
    return RCIN1_PIN;
  }
  else if(rc_input_number == RCIN2){
    return RCIN2_PIN;
  }
  
  //else
  return 0xff;
}
