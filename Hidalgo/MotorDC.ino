#define SLOW 150
#define FAST 50

enum Dir {RIGHT = HIGH, LEFT = LOW} Direction;

int SpeedRegulation (int Speedinc) // Function in charge of regulating the delay, so that it can accelerate/decelerate depending on the speed increase
{
  // DECIDE WICH RANGE OF SPEED IS OKEY TO GO "FULL" SPEED AND WICH NOT
  // Have in mind that it is in the loop, so it will naturally go changing the speed as the increase is reduced
  if(Speedinc > FAST) // if the increment is big enough, the delay should be high so that it goes slowly
  {
    return 40;
  }
  if(Speedinc < SLOW) // if the increment is small enough, the delay should be high so that it goes faster
  {
    return 5;
  }
  return 20; // by default it should goes 50
}

int CheckSpeed (int Speed, int Voltage) // Simple check so that the speed isn't negative or too high
{
  //DO A RELATION IN FUNCTION OF THE VOLTAGE WITH THE VALUES OF THE DATA SHEET
  if((Speed > 0)) // Checks if the speed is positive
  {
    switch (Voltage) {
      case 12:
      {
        if(Speed > 100)
          return 100; // if the voltage is 12 V the maximum speed is 100
         return Speed; // returns the speed
      }
      case 24:
      {
        if(Speed > 600)
          return 600; // if the voltage is 24 V the maximum speed is ...
        return Speed; // returns the speed
      }
    }
  }
  return 0;
}

class MotorDC
{
  int DIR; // pin used to decide in wich direction does the motor rotate
  int PWM; // pin used to decide the pwm for the motor
  int K; //Relación de transformación;
  float lastSpeed; //saves the last speed
  int lastDIR; // variable with the las direction
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
  int getPWM() // returns the PWM value
  {
    return PWM;
  }
  MotorDC(int dir, int pwm, float V = 12): DIR(dir), PWM(pwm), Voltage(V) {}
  void setDIR (int dir) // it changes the direction
  {
    if(lastDIR != DIR)
    {
      MotorDC::SetMotorSpeed(0);
    }
    DIR = dir;

  }
  void SetMotorSpeed(float Speed, bool brake = false) // the speed is supposed to come in rmp
  {
    int val; // value that's going to go between 0-255 in relation to the porcentual speed
    int lastval; // value that's going to go between 0-255 in relation to the porcentual last speed
    Speed = CheckSpeed(Speed, Voltage); // small check so that the speed makes sense

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
        if(!brake)
          delay(SpeedRegulation(abs)); // the delay varies depending of the increase

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
        if(!brake)
          delay(SpeedRegulation(abs)); // the delay varies depending of the increase
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
  M.setDIR(RIGHT);
  M.SetMotorSpeed(50);
  //delay(500);
  M.setDIR(LEFT);
  M.SetMotorSpeed(100);
  //delay(500);
  M.setDIR(RIGHT);
  M.SetMotorSpeed(25);
  //delay(500);
  M.setDIR(LEFT);
  M.SetMotorSpeed(75);
  //delay(500);
  M.setDIR(RIGHT);
}
