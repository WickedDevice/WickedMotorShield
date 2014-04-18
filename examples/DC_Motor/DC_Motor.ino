#include <WickedMotorShield.h>
WickedMotor motors;

void printCurrentSensing(void);

int num_motors = 4;
uint8_t m[] = {M1, M2, M3, M4};

//int num_motors = 6;
//uint8_t m[] = {M1, M2, M3, M4, M5, M6};

void setup(void){
  Serial.begin(115200);
  Serial.println(F("Wicked Motor Shield - DC Motors"));
  
  // note, library initialized all motors to a clockwise direction and brake condition
}

void loop(void){

  // for each motor individually run some tests
  for(int i = 0; i < num_motors; i++){
    Serial.println(F("================================"));
  
    // do some stuff clockwise
    motors.setDirection(m[i], DIR_CW); // clockwise
    motors.setBrake(m[i], BRAKE_OFF);  // no brake applied    
    
    Serial.print(F("Turning on Motor M"));
    Serial.print(i+1);
    Serial.println(F(", Full Speed, Clockwise"));
    motors.setSpeed(m[i], 255);        // full speed        
    delay(2000);
     
    Serial.print(F("Turning on Motor M"));
    Serial.print(i+1);
    Serial.println(F(", Half Speed, Clockwise"));
    motors.setSpeed(m[i], 127);        // half speed
    delay(2000);
  
    Serial.print(F("Soft Brake on Motor M"));
    Serial.print(i+1);
    Serial.println();
    motors.setBrake(m[i], BRAKE_SOFT); // soft brake applied    
    delay(2000);   

    Serial.print(F("Turning on Motor M"));
    Serial.print(i+1);
    Serial.println(F(", Full Speed, Clockwise"));    
    motors.setSpeed(m[i], 255);        // full speed
    motors.setBrake(m[i], BRAKE_OFF);  // no brake applied
    delay(2000);            
        
    Serial.print(F("Hard Brake on Motor M"));
    Serial.print(i+1);
    Serial.println();
    motors.setBrake(m[i], BRAKE_HARD);  // hard brake applied    
    delay(2000); 
    
    Serial.println(F("--------------------------------"));
    
    // do some stuff counter clockwise
    motors.setDirection(m[i], DIR_CCW); // counter clockwise
    motors.setBrake(m[i], BRAKE_OFF);   // no brake applied   
        
    Serial.print(F("Turning on Motor M"));
    Serial.print(i+1);
    Serial.println(F(", Full Speed, Counter Clockwise"));
    motors.setSpeed(m[i], 255);         // full speed
    delay(2000);
     
    Serial.print(F("Turning on Motor M"));
    Serial.print(i+1);
    Serial.println(F(", Half Speed, Counter Clockwise"));
    motors.setSpeed(m[i], 127);         // half speed
    motors.setBrake(m[i], BRAKE_OFF);   // no brake applied    
    delay(2000);    
  
    Serial.print(F("Soft Brake on Motor M"));
    Serial.print(i+1);
    Serial.println();
    motors.setBrake(m[i], BRAKE_SOFT);  // soft brake applied    
    delay(2000);   

    Serial.print(F("Turning on Motor M"));
    Serial.print(i+1);
    Serial.println(F(", Full Speed, Counter Clockwise"));
    motors.setSpeed(m[i], 255);         // full speed
    motors.setBrake(m[i], BRAKE_OFF);   // no brake applied    
    delay(2000);            
        
    Serial.print(F("Hard Brake on Motor M"));
    Serial.print(i+1);
    Serial.println();
    motors.setBrake(m[i], BRAKE_HARD);  // hard brake applied    
    delay(2000); 
  }
  
  // run all four motors at full speed , clockwise
  Serial.println(F("Running all four motors full speed, clockwise"));
  for(int i = 0; i < num_motors; i++){
    motors.setSpeed(m[i], 255);        // initialize full speed on all motors
    motors.setDirection(m[i], DIR_CW); // initialize all motors clockwise
    motors.setBrake(m[i], BRAKE_OFF);  // initialize all motors with brake applied
  }    
  printCurrentSensing(); // effectvely a one second delay
  

  // soft braking all four motors
  Serial.println(F("Soft braking all four motors"));
  for(int i = 0; i < num_motors; i++){
    motors.setBrake(m[i], BRAKE_SOFT);  // initialize all motors with brake applied
  }  
  printCurrentSensing(); // effectvely a one second delay
  
  // run all four motors at full speed , counter clockwise
  Serial.println(F("Running all four motors full speed, counter clockwise"));
  for(int i = 0; i < num_motors; i++){
    motors.setSpeed(m[i], 255);         // initialize full speed on all motors
    motors.setDirection(m[i], DIR_CCW); // initialize all motors clockwise
    motors.setBrake(m[i], BRAKE_OFF);   // initialize all motors with brake applied
  }   
  printCurrentSensing(); // effectvely a one second delay
  
  // soft braking all four motors
  Serial.println(F("Hard braking all four motors"));
  for(int i = 0; i < num_motors; i++){
    motors.setBrake(m[i], BRAKE_HARD);  // initialize all motors with brake applied
  }  
  printCurrentSensing(); // effectvely a one second delay  
}

//prints 10 ADC values for each input over about a second
void printCurrentSensing(void){
  Serial.println(F("Current Sensing"));
  Serial.println(F("M1\tM2\tM3\tM4"));
  for(int i = 0; i < 20; i++){
    for(int j = 0; j < num_motors; j++){
      Serial.print(motors.currentSense(m[j]));
      Serial.print("\t");
    }
    Serial.println();
    delay(100);
  }
}

uint32_t getRCIN(uint8_t rc_input_number){

}
