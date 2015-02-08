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

#ifndef _WICKED_STEPPER_H
#define _WICKED_STEPPER_H

#include <WickedMotorShield.h>
#include <stdint.h>

class Wicked_Stepper : public WickedMotorShield{
 private:
    void stepMotor(int this_step);

    uint8_t direction;             // Direction of rotation
    uint16_t speed;                // Speed in RPMs
    uint32_t step_delay;           // delay between steps, in ms, based on speed
    uint16_t number_of_steps;      // total number of steps this motor can take
    uint16_t step_number;          // which step the motor is on
    uint32_t last_step_time;       // time stamp in ms of when the last step was taken
    uint8_t m1;                    // the M-number of the first coil
    uint8_t m2;                    // the M-number of the second coil

 public:
   Wicked_Stepper(uint16_t number_of_steps, uint8_t m1, uint8_t m2, uint8_t use_alternate_pins = 0);
   void setSpeed(uint32_t speed);
   void step(int16_t number_of_steps);
};

#endif /* _WICKED_STEPPER_H */
