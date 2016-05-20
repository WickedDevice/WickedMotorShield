#include <WickedMotorShield.h>

void printCurrentSensing(void);

int num_motors = 4;
Wicked_DCMotor motor1(M1);
Wicked_DCMotor motor2(M2);
Wicked_DCMotor motor3(M3);
Wicked_DCMotor motor4(M4);

Wicked_DCMotor *m[] = {&motor1, &motor2, &motor3, &motor4};

/*
int num_motors = 6;
Wicked_DCMotor motor1(M1);
Wicked_DCMotor motor2(M2);
Wicked_DCMotor motor3(M3);
Wicked_DCMotor motor4(M4);
Wicked_DCMotor motor5(M5);
Wicked_DCMotor motor6(M6);
Wicked_DCMotor *m[] = {&motor1, &motor2, &motor3, &motor4, &motor5, &motor6};
*/

void setup(void){
  Serial.begin(115200);
  Serial.print(F("Wicked Motor Shield Library version "));
  Serial.print(WickedMotorShield::version());
  Serial.println(F("- DC Motors"));
  
  // note, library initialized all motors to a clockwise direction and brake condition
}

void loop(void){

  // for each motor individually run some tests
  for(int ii = 0; ii < num_motors; ii++){
    Serial.println(F("================================"));
  
    // do some stuff clockwise
    m[ii]->setDirection(DIR_CW); // clockwise
    m[ii]->setBrake(BRAKE_OFF);  // no brake applied    
    
    Serial.print(F("Turning on Motor M"));
    Serial.print(ii+1);
    Serial.println(F(", Full Speed, Clockwise"));
    m[ii]->setSpeed(255);        // full speed        
    delay(2000);
     
    Serial.print(F("Turning on Motor M"));
    Serial.print(ii+1);
    Serial.println(F(", Half Speed, Clockwise"));
    m[ii]->setSpeed(127);        // half speed
    delay(2000);
  
    Serial.print(F("Soft Brake on Motor M"));
    Serial.print(ii+1);
    Serial.println();
    m[ii]->setBrake(BRAKE_SOFT); // soft brake applied    
    delay(2000);   

    Serial.print(F("Turning on Motor M"));
    Serial.print(ii+1);
    Serial.println(F(", Full Speed, Clockwise"));    
    m[ii]->setSpeed(255);        // full speed
    m[ii]->setBrake(BRAKE_OFF);  // no brake applied
    delay(2000);            
        
    Serial.print(F("Hard Brake on Motor M"));
    Serial.print(ii+1);
    Serial.println();
    m[ii]->setBrake(BRAKE_HARD);  // hard brake applied    
    delay(2000); 
    
    Serial.println(F("--------------------------------"));
    
    // do some stuff counter clockwise
    m[ii]->setDirection(DIR_CCW); // counter clockwise
    m[ii]->setBrake(BRAKE_OFF);   // no brake applied   
        
    Serial.print(F("Turning on Motor M"));
    Serial.print(ii+1);
    Serial.println(F(", Full Speed, Counter Clockwise"));
    m[ii]->setSpeed(255);         // full speed
    delay(2000);
     
    Serial.print(F("Turning on Motor M"));
    Serial.print(ii+1);
    Serial.println(F(", Half Speed, Counter Clockwise"));
    m[ii]->setSpeed(127);         // half speed
    m[ii]->setBrake(BRAKE_OFF);   // no brake applied    
    delay(2000);    
  
    Serial.print(F("Soft Brake on Motor M"));
    Serial.print(ii+1);
    Serial.println();
    m[ii]->setBrake(BRAKE_SOFT);  // soft brake applied    
    delay(2000);   

    Serial.print(F("Turning on Motor M"));
    Serial.print(ii+1);
    Serial.println(F(", Full Speed, Counter Clockwise"));
    m[ii]->setSpeed(255);         // full speed
    m[ii]->setBrake(BRAKE_OFF);   // no brake applied    
    delay(2000);            
        
    Serial.print(F("Hard Brake on Motor M"));
    Serial.print(ii+1);
    Serial.println();
    m[ii]->setBrake(BRAKE_HARD);  // hard brake applied    
    delay(2000); 
  }
  
  // run all four motors at full speed , clockwise
  Serial.println(F("Running all four motors full speed, clockwise"));
  for(int ii = 0; ii < num_motors; ii++){
    m[ii]->setSpeed(255);        // initialize full speed on all motors
    m[ii]->setDirection(DIR_CW); // initialize all motors clockwise
    m[ii]->setBrake(BRAKE_OFF);  // initialize all motors with brake applied
  }    
  printCurrentSensing(); // effectvely a one second delay
  

  // soft braking all four motors
  Serial.println(F("Soft braking all four motors"));
  for(int ii = 0; ii < num_motors; ii++){
    m[ii]->setBrake(BRAKE_SOFT);  // initialize all motors with brake applied
  }  
  printCurrentSensing(); // effectvely a one second delay
  
  // run all four motors at full speed , counter clockwise
  Serial.println(F("Running all four motors full speed, counter clockwise"));
  for(int ii = 0; ii < num_motors; ii++){
    m[ii]->setSpeed(255);         // initialize full speed on all motors
    m[ii]->setDirection(DIR_CCW); // initialize all motors clockwise
    m[ii]->setBrake(BRAKE_OFF);   // initialize all motors with brake applied
  }   
  printCurrentSensing(); // effectvely a one second delay
  
  // soft braking all four motors
  Serial.println(F("Hard braking all four motors"));
  for(int ii = 0; ii < num_motors; ii++){
    m[ii]->setBrake(BRAKE_HARD);  // initialize all motors with brake applied
  }  
  printCurrentSensing(); // effectvely a one second delay  
}

//prints 20 ADC values for each input over about two seconds
void printCurrentSensing(void){
  Serial.println(F("Current Sensing"));
  Serial.println(F("M1\tM2\tM3\tM4"));
  for(int ii = 0; ii < 20; ii++){
    for(int j = 0; j < num_motors; j++){
      Serial.print(m[j]->currentSense());
      Serial.print("\t");
    }
    Serial.println();
    delay(100);
  }
}

uint32_t getRCIN(uint8_t rc_input_number){

}
