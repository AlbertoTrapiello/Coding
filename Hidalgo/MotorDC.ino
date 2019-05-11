

class MotorDC
{
  int DIR; // pin used to decide in wich direction does the motor rotate
  int PWM; // pin used to decide the pwm for the motor
  int K; //Relación de transformación;
  float lastSpeed; //saves the last speed
  float Voltage; //Voltage
public:
  MotorDC()
  {
    DIR = 2; // by default these is supposed to be the direction pin
    PWM = 3; // by default these is supposed to be the pwm pin
    Voltage = 12; // by default it's supposed to be connected to 12 V
    lastSpeed = 0; // by default it's supposed to start stopped
  }
  MotorDC(int dir, int pwm, float V = 12): DIR(dir), PWM(pwm), Voltage(V) {}
  void SetMotorSpeed(float speed) // the speed is supposed to come in rmp
  {
    int val; // value that's going to go between 0-255 in relation to the porcentual speed
    int lastval; // value that's going to go between 0-255 in relation to the porcentual last speed
    val = map(speed, 0, 100, 0, 255); // transforms the speed to the analog value
    lastval = map(lastSpeed, 0, 100, 0, 255); // transforms the last speed to the analog value
    if(val > lastval)
    {
      int abs = val - lastval; // determines the increase
      for (int i = lastval; i <= val; i++)
      {
        analogWrite(PWM, i); //accelerates "slowly"
        delay(50);
      }
    }
    else
    {
      int abs = lastval - val; // determines the increase
      for (int i = val; i >= lastval; i--)
      {
        analogWrite(PWM, i); //accelerates "slowly"
        delay(50);
      }
    }

  }

};

void setup() {
  // put your setup code here, to run once:
  pinMode(PWM, OUTPUT);
  pinMode(DIR, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(DIR, HIGH);

}
