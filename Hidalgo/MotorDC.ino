#define SLOW 150
#define FAST 50


//DC MOTOR CHARACTERISTICS
#define DC_V 12 //The motor's DC Voltage
#define NL_rpm 100  //The motor's rpm


enum Dir {RIGHT = 1, LEFT = 0} Direction;

double computePID (double in)
{
  double kp = 2;
  double ki = 5;
  double kd = 1;

  currentTime = millis(); // get current time
  elapsedTime = (double)(currentTime - previousTime); // compute time elapsed from previous computation

  error = Setpoint - inp; // determine error
  cumError += error * elapsedTime; // compute integral
  rateError = (error - lastError)/elapsedTime; // compute derivative

   double out = kp*error + ki*cumError + kd*rateError;  //PID output

   lastError = error; //remember current error
   previousTime = currentTime; //remember current time

   return out;
}

int SpeedRegulation (int Speedinc) // Function in charge of regulating the delay, so that it can accelerate/decelerate depending on the speed increase
{
  // DECIDE WICH RANGE OF SPEED IS OKEY TO GO "FULL" SPEED AND WICH NOT
  // Have in mind that it is in the loop, so it will naturally go changing the speed as the increase is reduced
  if(Speedinc > FAST) // if the increment is big enough, the delay should be small so that it goes faster
  {
    return 5;
  }
  if(Speedinc < SLOW) // if the increment is small enough, the delay should be high so that it goes slowly
  {
    return 40;
  }
  return 20; // by default it should goes 50
}

int CheckSpeed (int Speed, int Voltage) // Simple check so that the speed isn't negative or too high
{
  //DO A RELATION IN FUNCTION OF THE VOLTAGE WITH THE VALUES OF THE DATA SHEET
  if((Speed >= 0)) // Checks if the speed is positive
  {
    switch (Voltage) {
      case 12:
      {
        if(Speed > NL_rpm)
          return NL_rpm; // if the voltage is 12 V the maximum speed is 100
        return Speed; // returns the speed
      }
      /*case 24:
      {
        if(Speed > 600)
          return 600; // if the voltage is 24 V the maximum speed is ...
        return Speed; // returns the speed
      }
      INITIALLY IT DOESN'T MATTER THE VOLTAGE, IT JUST MATTER THE RALTIONSHIP BETWEEEN VOLTAGE AND RMP, DEPENDING ON THE TABLE*/
    }
  }
  Serial.println("ERROR Speed can't be negative");
  return -1;
}

//POTENTIOMETER
class Potentiometer
{
  int pin_POT; // pin use to read the potentiometer analog input

public:
  //CONSTRUCTOR
  Potentiometer()
  {
    pin_POT = A0; // by default these is supposed to be the potentiometer's pin
  }

  //FUNCTIONS
  int getpin_POT()
  {
    return pin_POT;
  }
  float ReadAngle()
  {
    float angle; // value of the angle read by the analog input
    angle = map(analogRead(pin_POT), 0, 360, 0, 255); // translates the value of the reading to an angle
    if(angle >= 0)
      return angle;
    Serial.println("ERROR Angle here can't be negative");
    return -1; // ERROR
  }
};

//MOTOR
class MotorDC
{
  //PINS
  int pin_DIR; // pin used to decide in wich direction does the motor rotate
  int pin_PWM; // pin used to decide the PWM for the motor
  int pin_POS; // pin used to read the position

  //VARIABLES
  int K = 0; // Relación de transformación;
  int lastDIR = 0; // variable with the las direction
  int BOOST  = 1; // Variable that determines the speed at wich the motor accelerates or deccelerates
  float lastSpeed = 0; //saves the last speed
  float lastAngle = 0; // value of the angle
  int val; // value that's going to go between 0-255 in relation to the porcentual speed
  int lastval; // value that's going to go between 0-255 in relation to the porcentual last speed
  float Voltage = 0; // Voltage

  //POTENTIOMETER
  Potentiometer potentiometer; //object from the class potentiometer, used to establish the angle of the rotation

public:

  //CONSTRUCTOR
  MotorDC():potentiometer()
  {
     pin_DIR= 2; // by default these is supposed to be the direction pin
     pin_PWM = 3; // by default these is supposed to be the pwm pin
     pin_POS = potentiometer.getpin_POT(); // it reads the supposed pin for the potentiometer
     Voltage = 12; // by default it's supposed to be connected to 12 V
     lastSpeed = 0; // by default it's supposed to start stopped
     lastAngle = potentiometer.ReadAngle(); // start reading the position
  }

  MotorDC(int dir, int pwm, float V = DC_V): potentiometer(), pin_DIR(dir), pin_PWM(pwm), Voltage(V) {}

  //FUNCTIONS

  int getpin_DIR() // returns the DIR value
  {
    return pin_DIR;
  }

  int getpin_PWM() // returns the PWM value
  {
    return pin_PWM;
  }
    int getpin_POT()
  {
    return potentiometer.getpin_POT();
  }
  void setDIR (int dir, bool brake = true) // it changes the direction. taking care of slowing down first if needed.
  {
    if(brake) //in case it needs to brake
    {
      if(lastDIR != dir) // if it has to change the direction, it first has to slow down
      {
        Serial.println();
        Serial.println("SLOWING DOWN");
        Serial.println();
        MotorDC::SetMotorSpeed(0); // slows down before changing the direction
      }
    }
    lastDIR = dir; // actualize the lastDIR value
    digitalWrite(pin_DIR, dir); // set direction in wich the motor turns
  }
  void SetMotorPosition(float angle) // rotates the motor until de potentiometer says that it just reached the angle
  {
    float portion; // variable that affects the speed at wich the motor turns

    if(angle > lastAngle) // if the angle needed is higher than the actual angle, I assume that the motor needs to turn RIGHT
    {
      MotorDC::setDIR(RIGHT); // determines the turning Direction
    }
    else
    {
      MotorDC::setDIR(LEFT); // determines the turning Direction
    }
    Speed = computePID(angle);
    /*
    while(potentiometer.ReadAngle() != angle)
    {
      if(potentiometer.ReadAngle() != -1)
      {
        portion = NL_rpm*(angle - potentiometer.ReadAngle())/360; // calculation of the portion so that when the angle is far it goes faster
        SetMotorSpeed(portion); // it modifies the speed, so that when the angle is close it slows
      }
    }
    */
    lastAngle = potentiometer.ReadAngle(); // updates the value of the last Angle to the actual Angle;
  }

  void SetMotorSpeed(float Speed) // the speed is supposed to come in rmp
  {
    if (CheckSpeed(Speed, Voltage))// small check so that the speed makes sense
    {
      val = map(Speed, 0, NL_rpm, 0, 255); // transforms the speed to the analog value
      lastval = map(lastSpeed, 0, NL_rpm, 0, 255); // transforms the last speed to the analog value
      Serial.println("----------------");
      Serial.print("Speed: ");
      Serial.println(Speed);
      Serial.print("lastSpeed: ");
      Serial.println(lastSpeed);
      Serial.print("val: ");
      Serial.println(val);
      Serial.print("lastval: ");
      Serial.println(lastval);

      int abs = val - lastval; //determines the increase

      if(val > lastval) // If the Speed is greater than the last recorded Speed
      {
        for (int i = lastval; i <= val; i = i + BOOST) // it cycles at certain rate, depending on the boost value
        {
          analogWrite(pin_PWM, i); //accelerates "slowly"
          Serial.print("accelarating ");
          Serial.println(i);
          delay(SpeedRegulation(abs)); // the delay varies depending of the increase
        }
      }
      else if (val < lastval)
      {
        for (int i = lastval; i >= val; i = i - BOOST) // it cycles at certain rate, depending on the boost value
        {
          if(i < 0) // if the value is negative it prevents the motor from having an error
            i = 0;
          Serial.print("decelarating ");
          Serial.println(i);
          analogWrite(pin_PWM, i); //decelerates "slowly"
          delay(SpeedRegulation(abs)); // the delay varies depending of the increase
        }
      }

    }
    lastSpeed = Speed; // updates the value
  }

};

MotorDC M;

void setup() {
  // put your setup code here, to run once:

  pinMode(M.getpin_PWM(), OUTPUT); //establish the pin for the PWM
  pinMode(M.getpin_DIR(), OUTPUT); //establish the pin for the DIR
  pinMode(M.getpin_POT(),INPUT); //establish the pin for the POT

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  M.setDIR(RIGHT, true);
  M.SetMotorSpeed(50);
  //delay(500);
  M.setDIR(LEFT, true);
  M.SetMotorSpeed(100);
  //delay(500);
  M.setDIR(RIGHT, true);
  M.SetMotorSpeed(25);
  //delay(500);
  M.setDIR(LEFT, true);
  M.SetMotorSpeed(75);
  //delay(500);
  M.setDIR(RIGHT, true);
}
