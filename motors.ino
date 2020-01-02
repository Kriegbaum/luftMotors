// Basic sketch for trying out the Adafruit DRV8871 Breakout

#define MOTOR_IN1 5
#define MOTOR_IN2 6

class Wheel {
  private:
    byte posPin;
    byte negPin;
    float dutyCycle;
    bool state;
    bool engaged;
    byte cycleTime;
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

    bool tick(unsigned long currentMillis){
      if (not state) {
        return false;
      }
      
      else {
        if (engaged) {
          float interval = cycleTime - (dutyCycle * cycleTime);
          if (currentMillis - previousMillis >= interval) {
            digitalWrite(negPin, HIGH);
            engaged = false;
            previousMillis = millis();
            return true;
          }
        }
        else{
          float interval = dutyCycle * cycleTime;
          if (currentMillis - previousMillis >= interval) {
            digitalWrite(negPin, LOW);
            engaged = true;
            previousMillis = millis();
            return true;
          }
        }
      }
    }
};

Wheel wheel1 = Wheel(5, 6, 1500);

void tickDelay(unsigned long delayTime) {
  unsigned long delayStart = millis();
  bool ticking = true;
  while(ticking){
    unsigned long currentMillis = millis();
    wheel1.tick(currentMillis);
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
  unsigned long currentMillis = millis();
  wheel1.motorSpeed(0.6);
  wheel1.on();
  Serial.println("Motor on");
  tickDelay(30000);
  wheel1.off();
  Serial.println("Motor off");
  tickDelay(20000);
  
}
