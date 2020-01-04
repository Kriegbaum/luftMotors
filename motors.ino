// Basic sketch for trying out the Adafruit DRV8871 Breakout

#define NUMBEROFWHEELS 2

class Wheel {
  private:
    byte posPin;
    byte negPin;
    //Percent form of current speed 
    float dutyCycle;
    //Is motor running?
    bool state;
    //Is motor getting power in the enabled part of the duty cycle?
    bool engaged;
    int cycleTime;
    //Last time in milliseconds that the program has checked in on the motor
    unsigned long previousMillis;
  public: 
    Wheel(byte pos, byte neg, byte cycle) { 
      posPin = pos;
      negPin = neg;
      dutyCycle = 0.5;
      cycleTime = cycle;
      state = false;
      engaged = false;
      previousMillis = millis();
      pinMode(posPin, OUTPUT);
      pinMode(negPin, OUTPUT);
      digitalWrite(posPin, LOW);
      digitalWrite(negPin, HIGH);
    }
    
    void on() {
      state = true;
    }
    
    void off() {
      digitalWrite(negPin, LOW);
      state = false;
    }
    
    void toggle() {
      if (state) {
        state = false;
      }
      else {
        state = true;
      }
    }
    
    void motorSpeed(float motorSpeed) {
      dutyCycle = motorSpeed;
    }

  bool getState() {
      return state;
  }

    void tick(){
      if (not state) {
        return;
      }
      
      else {
        unsigned long currentMillis = millis();
        if (engaged) {
          float interval = cycleTime - (dutyCycle * cycleTime);
          if (currentMillis - previousMillis >= interval) {
            digitalWrite(negPin, HIGH);
            engaged = false;
            previousMillis = millis();
            return;
          }
        }
        else{
          float interval = dutyCycle * cycleTime;
          if (currentMillis - previousMillis >= interval) {
            digitalWrite(negPin, LOW);
            engaged = true;
            previousMillis = millis();
            return;
          }
        }
      }
    }
};



////////////////DECLARE WHEELS HERE////////////////

Wheel wheels[NUMBEROFWHEELS] = 
  {Wheel(7, 8, 500),
  Wheel(5, 6, 500)}; 

/////////////END WHEEL DECLARATION//////////////////



void wheelsOnOff(Wheel *wheels, bool state){
  for(int i=0; i<NUMBEROFWHEELS; i++){
    if (state) {
      wheels[i].on();
    }
    else {
      wheels[i].off();
    }
  }
}

void tickDelay(unsigned long delayTime) {
  unsigned long delayStart = millis();
  while(1){
    unsigned long currentMillis = millis();
    for(int i=0; i < NUMBEROFWHEELS; i++){
      wheels[i].tick();
    }
    if (currentMillis - delayStart >= delayTime){
      break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Motor Test Initiated");
}

void loop() {
  wheels[0].motorSpeed(0.6);
  wheelsOnOff(wheels, true);
  Serial.println("Motor on");
  tickDelay(30000); 
  Serial.println("Motor off");
  wheelsOnOff(wheels, false);
  tickDelay(32000);
}
