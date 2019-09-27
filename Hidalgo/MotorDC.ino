#define SLOW 150
#define FAST 50


//DC MOTOR CHARACTERISTICS
#define DC_V 12 //The motor's DC Voltage
#define NL_rpm 100  //The motor's rpm

enum Direction {RIGHT = 1, LEFT = 0};

/*double computePID (double inp)
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
}*/

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
  void PrintAngle ()
  {
    Serial.print("The Actual Value of the angle is: ");
    Serial.println(Potentiometer::ReadAngle()); // prints the value of the angle readed
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
  // int BOOST  = 1; // Variable that determines the speed at wich the motor accelerates or deccelerates
  float lastSpeed = 0; //saves the last speed
  float lastAngle = 0; // value of the angle
  float zero = 0; // value of the analogRead that it suppoused to be de origin
  int val; // value that's going to go between 0-255 in relation to the porcentual speed
  int lastval; // value that's going to go between 0-255 in relation to the porcentual last speed
  float Voltage = 0; // Voltage
  int totalAngle = 0; // Value of the accumulated turns
  int zeroAngle = 0; // Value of the analogRead of the Zero
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
     zeroAngle = lastAngle = potentiometer.ReadAngle(); // start reading the position
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
    /*
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
    */
    lastDIR = dir; // actualize the lastDIR value
    digitalWrite(pin_DIR, dir); // set direction in wich the motor turns
  }
  void SetMotorPosition(float angle) // rotates the motor until de potentiometer says that it just reached the angle
  {
    int time_delay = 0; // Time that the motor takes to turn 360º
    int Speed = NL_rpm*1; // Speed at wich the motor turns
    time_delay = 1000/Speed; // Calculates the time it takes to make a turn

    Serial.print("TIME PER REV = ");
    Serial.println(time_delay);

    if(angle > 0 )
    {
      MotorDC::setDIR(RIGHT); // assuming the positive value should be RIGHT
    }
    else
    {
      MotorDC::setDIR(LEFT); // assuming the negative value should be LEFT
    }

    int pwm_speed = map (Speed, 0, NL_rpm, 0, 255); // Translates the Speed to an analog value for the PWM signal
    analogWrite(pin_PWM, pwm_speed); // Sets the motor to rotate full speed
    delay(time_delay); // Keeps the motor turning during the estimated time_delay
    analogWrite(pin_PWM, 0); // Stops the motor from turning

    potentiometer.PrintAngle(); // prints the value of the angle

    totalAngle += (angle - lastAngle); // accumulates the angle at which the motor is right now
    lastAngle = angle; // updates the angle to the current value


  }
  void TurnToZero () // Function that allows the motor to return to 0;
  {
    MotorDC::SetMotorPosition(-totalAngle); // turns to the opposite side the same amount that it had turned in total
  }
  void TurnBack () // Turns in the other direction the same amount of angle that earlier was turned
  {
    MotorDC::SetMotorPosition(-lastAngle); // turns the same amount that he has turned the last time
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

//Changed the code so I can try to make the system's model

void loop() {
  // put your main code here, to run repeatedly:

  //TURNS 45 DEGREES
  M.SetMotorPosition(45);
  delay(500);
  M.TurnBack(); // Goes Back the same amount that it turned last time
  //TURNS 90 DEGREES
  M.SetMotorPosition(90);
  delay(500);
  M.TurnBack();
  //TURNS 180 DEGREES
  M.SetMotorPosition(180);
  delay(500);
  M.TurnBack();
  //TURNS 360 DEGREES
  M.SetMotorPosition(360);
  delay(500);
  M.TurnBack();
}
