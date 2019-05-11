

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
  int getDIR()
  {
    return DIR;
  }
  int getPWM()
  {
    return PWM;
  }
  MotorDC(int dir, int pwm, float V = 12): DIR(dir), PWM(pwm), Voltage(V) {}
  void SetMotorSpeed(float Speed) // the speed is supposed to come in rmp
  {
    int val; // value that's going to go between 0-255 in relation to the porcentual speed
    int lastval; // value that's going to go between 0-255 in relation to the porcentual last speed
    val = map(Speed, 0, 100, 0, 255); // transforms the speed to the analog value
    lastval = map(lastSpeed, 0, 100, 0, 255); // transforms the last speed to the analog value
    Serial.println("----------------");
    Serial.print("val: ");
    Serial.println(val);
    Serial.print("lastval: ");
    Serial.println(lastval);

    if(val > lastval)
    {
      int abs = val - lastval; // determines the increase
      for (int i = lastval; i <= val; i++)
      {
        analogWrite(PWM, i); //accelerates "slowly"
        Serial.print("accelarating ");
        Serial.println(i);
        delay(50);
      }
    }
    else
    {
      int abs = lastval - val; // determines the increase
      for (int i = lastval; i >= val; i--)
      {
        analogWrite(PWM, i); //decelerates "slowly"
        Serial.print("decelarating ");
        Serial.println(i);
        delay(50);
      }
    }
  lastSpeed = Speed; // updates the value
  }

};

MotorDC M;

void setup() {
  // put your setup code here, to run once:


  pinMode(M.getPWM(), OUTPUT);
  pinMode(M.getDIR(), OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(M.getDIR(), HIGH);
  M.SetMotorSpeed(50);
  delay(500);
  M.SetMotorSpeed(100);
  delay(500);
  M.SetMotorSpeed(25);
  delay(500);
  M.SetMotorSpeed(75);
  delay(500);
}
