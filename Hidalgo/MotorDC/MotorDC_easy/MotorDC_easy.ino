//INCLUDES
#include <TimedAction.h>
#include <Vector.h>

//DEFINES
#define SLOW 150
#define FAST 50
#define MAX_ANGLE 340 // maximum angle that the potentiometer can measure correctly
#define MAX_POT 1023 // maximum value of the analog read available for the potentiometer
#define MIN_POT 0 // minimum value of the analog read available for the potentiometer
#define MAX_CHECK 4 // number of consecutive checks that the potentiometer does before assuring is a correct value
#define INC 2 // small increment to check the noise to avoid false readings
#define TIME_READ 1 // time between readings so that it can detect an error
#define T_GRAF 3000 // time it waits after the motor have reached the position to have more data for the model

//DC MOTOR CHARACTERISTICS
#define DC_V 12 //The motor's DC Voltage
#define NL_rpm 100  //The motor's rpm

enum Direction {RIGHT = 1, LEFT = 0, NONE = -1};

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
  int input; // variable used to store the analog reads
  int lastInput; // variable used to store the previous read
  float angle; // value of the angle read by the analog input
  int error = 0; // variable that helps the potentiometer check the noise
  
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
  float ReadAngle(Direction Dir = NONE)
  {
    if(Potentiometer::CheckNoise(Dir))
    {
      angle = map(input, MIN_POT, MAX_POT, 0, MAX_ANGLE); // translates the value of the reading to an angle
      /*Serial.print("input = ");
      Serial.println(input);      
      Serial.print("angle = ");
      Serial.println(angle);*/
      return angle; // ERROR
    }
    angle = -1;
    return -1;
  }
  bool CheckNoise(Direction Dir = RIGHT) // Function responsible to avoid the fake values
  {
    bool output[MAX_CHECK];
    int error = 0;

    for(int i = 0; i < MAX_CHECK; i++)
      output[i] = false;
  
    for(int i = 0; i < MAX_CHECK; i++)
    {
      input = analogRead(pin_POT); // reads the analog value of the potentiometer
      
      /*Serial.println("**********************");
      Serial.println(i);
      Serial.print("LAST INPUT: ");
      Serial.println(lastInput); // prints the value of the analogRead
      Serial.print("INPUT: ");
      Serial.println(input); // prints the value of the analogRead*/

      if(Dir == RIGHT)
      {
        //Serial.println("RIGHT");
        if(input < lastInput)
          output[i] = false;
        if (abs(input - lastInput) < INC)
          output[i] = true;
        else if (input >= lastInput)
          output[i] = true;
      }
      else if(Dir == LEFT)
      {
        //Serial.println("LEFT");
        if(input > lastInput)
          output[i] = false;
        if (abs(input - lastInput) < INC)
          output[i] = true;
        else if (input <= lastInput)
          output[i] = true;
      }
      else
        output[i] = false;
        
      lastInput = input;

      delay(TIME_READ); // waits a little bit between readings so that it can detect a possible error
      
      /*Serial.print("Output[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(output[i]);*/
    }
    for(int i = 0; i< MAX_CHECK; i++)
    {
      if(output[i] == false)
        error++;
    }
  
    /*Serial.print("ERROR = ");
    Serial.println(error);*/
    if(error > 0 )
      return false;
    else
      return true;
  }
  void PrintAngle (Direction Dir = NONE)
  {
    if(angle >= 0)
    {
      //Serial.print("The Actual Value of the angle is: ");
      Serial.print(angle); // prints the value of the angle readed
    }
    else
      Serial.print("***");
      //Serial.println("Value probably between 340 and 360 degrees, wich the potentiometer can't read");
    Serial.print(";");
    Serial.println(millis());
  }
};

Potentiometer P; // the potentiometer object

//MOTOR
class MotorDC
{
  //PINS
  int pin_DIR; // pin used to decide in wich direction does the motor rotate
  int pin_PWM; // pin used to decide the PWM for the motor
  int pin_POS; // pin used to read the position

  //VARIABLES
  float K = (57.0/23.0)*(32.493/45.0); // Relación de transformación;
  enum Direction lastDIR = NONE; // variable with the las direction
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
  //Potentiometer potentiometer; //object from the class potentiometer, used to establish the angle of the rotation
  int POT_measure = 0; // variable wich saves the value of the pot reading
  float time_1 = 0; // time used to avoid delay()
  float time_2 = 0; // time used to avoid delay()

public:

  //CONSTRUCTOR
  MotorDC()
  {
     pin_DIR= 2; // by default these is supposed to be the direction pin
     pin_PWM = 3; // by default these is supposed to be the pwm pin
     //pin_POS = potentiometer.getpin_POT(); // it reads the supposed pin for the potentiometer
     Voltage = 12; // by default it's supposed to be connected to 12 V
     lastSpeed = 0; // by default it's supposed to start stopped
     //zeroAngle = lastAngle = potentiometer.ReadAngle(NONE); // start reading the position
  }

  MotorDC(int dir, int pwm, float V = DC_V): pin_DIR(dir), pin_PWM(pwm), Voltage(V) {}

  //FUNCTIONS

  int getpin_DIR() // returns the DIR value
  {
    return pin_DIR;
  }

  int getpin_PWM() // returns the PWM value
  {
    return pin_PWM;
  }

  enum Direction getDIR()
  {
    return lastDIR;
  }
  
  void setDIR (enum Direction dir, bool brake = true) // it changes the direction. taking care of slowing down first if needed.
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
  void SetMotorPosition(float angle) // rotates the motor without feedback
  {
    float time_delay = 0; // Time that the motor takes to turn 360º
    float Speed = NL_rpm*1; // Speed at wich the motor turns
    time_delay = (60000/Speed)*(angle/360)*K; // Calculates the time it takes to make a turn 

    /*Serial.print("TIME PER REV = ");
    Serial.println(time_delay);*/


    if(angle > lastAngle)
    {
      MotorDC::setDIR(RIGHT); // assuming the positive value should be RIGHT
    }
    else
    {
      MotorDC::setDIR(LEFT); // assuming the negative value should be LEFT
    }

    if(time_delay < 0)
      time_delay = -1*time_delay;

    int pwm_speed = map (Speed, 0, NL_rpm, 0, 255); // Translates the Speed to an analog value for the PWM signal
    time_1 = millis();
    time_2 = millis() + time_delay;
    while(time_2 - time_1 >= 0)
    {
      analogWrite(pin_PWM, pwm_speed); // Sets the motor to rotate full speed
      time_1 = millis();
      P.ReadAngle(MotorDC::getDIR()); // reads the Angle of the potentiometer, and it should notify the Observers
      P.PrintAngle(); // to check the value with the updated one
    }   
    analogWrite(pin_PWM, 0); // Stops the motor from turning

    //potentiometer.PrintAngle(); // prints the value of the angle

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
  /*void ReadAngle () // SHOULD DO THE UPDATE
  {
    potentiometer.ReadAngle(lastDIR); // Reads the actual angle of the potentiometer
    potentiometer.PrintAngle(); // pritns the value readed
  }*/
  void update(int & v)
  {
    POT_measure = val; // saves the value of the measure in the variable
    Serial.println("Updated the value correctly");
    Serial.println(val);
  }
};

//OBSERVABLE



MotorDC M; // the motor object

/*
//MOTOR'S FUNCTION
void MotorFunction()
{
  /*
  //TURNS 45 DEGREES
  M.SetMotorPosition(45);
  delay(500);
  //M.ReadAngle();
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
*/
// THREADS SETUP
// It's suppoused that the main loop should have the least demanding task, iin this case the potentiometer's read
//TimedAction MotorThread = TimedAction(700, MotorFunction); // creates a thread that is going to run the motor's code

void setup() {
  // put your setup code here, to run once:

  pinMode(M.getpin_PWM(), OUTPUT); //establish the pin for the PWM
  pinMode(M.getpin_DIR(), OUTPUT); //establish the pin for the DIR
  pinMode(P.getpin_POT(), INPUT); //establish the pin for the POT

  Serial.begin(9600);
}
//to avoid using delay()

float t_1 = millis(); // time used to avoid delay()
float t_2 = 0; // time used to avoid delay() 

//Changed the code so I can try to make the system's model

void loop() {
  // put your main code here, to run repeatedly:

  /*
  //TURNS 45 DEGREES
  M.SetMotorPosition(45);
  delay(500);
  //M.ReadAngle();
  M.TurnBack(); // Goes Back the same amount that it turned last time
  //TURNS 90 DEGREES
  M.SetMotorPosition(90);
  delay(500);
  M.TurnBack();
  //TURNS 180 DEGREES
  M.SetMotorPosition(180);
  delay(500);
  M.TurnBack();*/
  //TURNS 180 DEGREES
  
  Serial.println("********+90********");
  M.SetMotorPosition(90);
  //delay(500); 
  t_2 = millis() + T_GRAF;
  while(t_2 - t_1 >= 0)
  {
    t_1 = millis();
    P.ReadAngle(M.getDIR()); // reads the Angle of the potentiometer, and it should notify the Observers
    P.PrintAngle(); // to check the value with the updated one
  } 
  Serial.println("********-90********");  
  M.TurnBack();
  //delay(500);
  t_2 = millis() + T_GRAF;
  while(t_2 - t_1 >= 0)
  {
    t_1 = millis();
    P.ReadAngle(M.getDIR()); // reads the Angle of the potentiometer, and it should notify the Observers
    P.PrintAngle(); // to check the value with the updated one
  } 
  
}
