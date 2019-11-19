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

class Observer // clase interfaz para los observadores
{
protected:
  int Index = -1; // index wich allows to find each observer in the vector

public:
  int getIndex()
  {
    return Index; // returns the value of the index
  }
  void setIndex(int i)
  {
    Index = i; // sets the value for the index
  }
  ~Observer()
  {

  }
  virtual void update(int & v) = 0; // every observer can react different to the changes
};

//MOTOR
class MotorDC: public Observer
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
  //Potentiometer potentiometer; //object from the class potentiometer, used to establish the angle of the rotation
  int POT_measure = 0; // variable wich saves the value of the pot reading

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
  void SetMotorPosition(float angle) // rotates the motor without feedback
  {
    int time_delay = 0; // Time that the motor takes to turn 360º
    int Speed = NL_rpm*1; // Speed at wich the motor turns
    time_delay = 1000/Speed; // Calculates the time it takes to make a turn

    Serial.print("TIME PER REV = ");
    Serial.println(time_delay);

    if(angle > lastAngle)
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

class Observable
{
  bool changed; // variable wich states if the observable has Changed
  int value; // value to be observed, so that when it changes the Observer can be notified
  Vector <MotorDC> observers; // list of observers
  int index = 0; // index of the recently created object of the vector

/*
protected:
  void setChanged() // NO IDEA
  {

  }
  void clearChanged() // NO IDEA
  {

  }
*/

public:
  ~Observable()
  {

  }
  void setValue(int v)
  {
    value = v; // changes the value to the new value
    Observable::hasChanged(); // notifies the observers
    ////cout << "New value: " << value << endl;
  }
  int getValue()
  {
    return value; // returns the value
  }
  void decreaseIndex()
  {
    for (int i = 0; i < observers.size(); i++)
    {
      if (observers[i].getIndex() > 0)
      {
        //cout << "The observer[" << i << "] had previously as index: " << observers[i].getIndex() << endl;
        observers[i].setIndex(observers[i].getIndex() - 1); // it reduces in 1 all the indexes from all the elements from the vector
        //cout << "The observers[" << i << "]'s index is: " << observers[i].getIndex() << endl;
      }
    }
  }
  void addObserver(MotorDC & o)
  {
    o.setIndex(index); // each MotorDC of the vector has it's own position
    observers.push_back(o); // when a new MotorDC wish to observe the variable it needs to attach to the vector so it can be notified as the rest
    //cout << "The index given to this observer is: " << index << " and the observer has " << o.getIndex() << endl;
    index++; // each time an element is deleted the index increases
  }
  void deleteObserver(MotorDC & o)
  {
    if (o.getIndex() < index)
    {
      //cout << "Deleted observer has " << o.getIndex() << " as index, and the actual index is " << index << endl;
      observers.remove(&observers.front() + o.getIndex()); // erases the vector's object wich position is the one dictated by the object
      Observable::decreaseIndex(); // it changes all the indexes from the vector
      index--; // each time an element is deleted the index decreases
      //cout << "The new index is: " << index << endl;
    }
    else
      //cout << "Couldn't delete the observer" << endl; //
  }
  void deleteObservers(MotorDC & o)
  {
    observers.clear(); // deletes all the objects from the vector
    index = 0; // if all the elements of the vector are deleted tha inde goes to 0
  }
  int countObservers()
  {
    return observers.size(); // returns the size of the vector
  }
  void hasChanged()
  {
    Observable::notifyObservers(value); // when a changes happends it notifies the changes to the observers
  }
  void notifyObservers(int & arg)
  {
    for (int i = 0; i < observers.size(); i++)
      observers[i].update(value); // for each element from the vector, it should be responsible of updating himself
  }
};

//POTENTIOMETER
class Potentiometer: public Observable
{
  int pin_POT; // pin use to read the potentiometer analog input
  int input; // variable used to store the analog reads
  int lastInput; // variable used to store the previous read

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
    float angle; // value of the angle read by the analog input
    if(Potentiometer::CheckNoise(Dir))
    {
      angle = map(input, 0, 360, 0, 255); // translates the value of the reading to an angle
      Observable::hasChanged(); // every time it reads the value of the potentiomenter it notifies the observers
    }
    Serial.println("ERROR Angle here can't be negative");
    return -1; // ERROR
  }
  bool CheckNoise(Direction Dir = NONE) // Function responsible to avoid the fake values
  {
    bool output[MAX_CHECK];
    int error = 0;

    for(int i = 0; i < MAX_CHECK; i++)
    {
      input = analogRead(A0);

      /*
      //PRINTED CHECK
      Serial.println("**********************");
      Serial.println(i);
      Serial.print("INPUT: ");
      Serial.println(input); // prints the value of the analogRead
      Serial.print("LAST INPUT: ");
      Serial.println(lastInput); // prints the value of the analogRead
      Serial.println("**********************");*/
      if(Dir != NONE)
      {
        if(Dir == RIGHT)
        {
          if(input >= lastInput)
            output[i] = true;
          else
            output[i] = false;
        }
        else if(Dir == LEFT)
        {
          if(input <= lastInput)
            output[i] = true;
          else
            output[i] = false;
        }
        else
          output[i] = false;
        lastInput = input;
      }
      //PRINTED CHECK
      /*
      Serial.print("Output[i]: ");
      Serial.println(output[i]);
      */
    }
    for(int i = 0; i< MAX_CHECK; i++)
    {
      if(output[i] == false)
        error++;
    }
    /*
    Serial.print("ERROR = ");
    Serial.println(error);
    */
    if(error > 0 )
      return false;
    else
      return true;
  }
  void PrintAngle (Direction Dir = NONE)
  {
    Serial.print("The Actual Value of the angle is: ");
    Serial.println(map(lastInput, 0, MAX_ANGLE, MIN_POT, MAX_POT)); // prints the value of the angle readed
  }
};

MotorDC M; // the motor object
Potentiometer P; // the potentiometer object

//MOTOR'S FUNCTION
void MotorFunction()
{
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

// THREADS SETUP
// It's suppoused that the main loop should have the least demanding task, iin this case the potentiometer's read
TimedAction MotorThread = TimedAction(700, MotorFunction); // creates a thread that is going to run the motor's code

void setup() {
  // put your setup code here, to run once:

  pinMode(M.getpin_PWM(), OUTPUT); //establish the pin for the PWM
  pinMode(M.getpin_DIR(), OUTPUT); //establish the pin for the DIR
  pinMode(P.getpin_POT(), INPUT); //establish the pin for the POT

  Serial.begin(9600);
}

//Changed the code so I can try to make the system's model

void loop() {
  // put your main code here, to run repeatedly:

  P.addObserver(M); // adds the observer to the vector of the observable

  P.ReadAngle(); // reads the Angle of the potentiometer, and it should notify the Observers
  P.PrintAngle(); // to check the value with the updated one

  // THREADS CHECK
  MotorThread.check(); // checks the thread, each loop it must check at least one time

}
