#define SLOW 150
#define FAST 50
#define BOOST 2

//DC MOTOR CHARACTERISTICS
#define DC_V 12 //The motor's DC Voltage
#define NL_rpm 100  //The motor's rpm


enum Dir {RIGHT = 1, LEFT = 0} Direction;

int SpeedRegulation (int Speedinc) // Function in charge of regulating the delay, so that it can accelerate/decelerate depending on the speed increase
{
  // DECIDE WICH RANGE OF SPEED IS OKEY TO GO "FULL" SPEED AND WICH NOT
  // Have in mind that it is in the loop, so it will naturally go changing the speed as the increase is reduced
  if(Speedinc > FAST) // if the increment is big enough, the delay should be high so that it goes slowly
  {
    return 40;
  }
  if(Speedinc < SLOW) // if the increment is small enough, the delay should be small so that it goes faster
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
        if(Speed > NL_rpm)
          return NL_rmp; // if the voltage is 12 V the maximum speed is 100
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
  return 0;
}

class MotorDC
{
  //PINS
  int pin_DIR; // pin used to decide in wich direction does the motor rotate
  int pin_PWM; // pin used to decide the PWM for the motor
  int pin_POS; // pin used to read the position

  //VARIABLES
  int K = 0; //Relación de transformación;
  float lastSpeed = 0; //saves the last speed
  int lastDIR = 0; // variable with the las direction
  //float Voltage = 0; // Voltage
  float lastangle = 0; // value of the angle

public:
  //CONSTRUCTOR
  MotorDC()
  {
     pin_DIR= 2; // by default these is supposed to be the direction pin
     pin_PWM = 3; // by default these is supposed to be the pwm pin
     Voltage = 12; // by default it's supposed to be connected to 12 V
     lastSpeed = 0; // by default it's supposed to start stopped
     lastangle = analogRead(POT); // start reading the position
  }

  MotorDC(int dir, int pwm, float V = DC_V): DIR(dir), PWM(pwm), Voltage(V) {}

  //FUNCTIONS

  int getDIR() // returns the DIR value
  {
    return DIR;
  }

  int getPWM() // returns the PWM value
  {
    return PWM;
  }

  void setDIR (int dir, bool brake = true) // it changes the direction. taking care of slowing down first if needed.
  {
    if(brake) //in case it needs to brake
    {
      if(lastDIR != dir) // if it has to change the direction, it first has to slow down
      {
        MotorDC::SetMotorSpeed(0); // slows down before changing the direction
      }
    }
    lastDIR = dir; // actualize the lastDIR value
    digitalWrite(DIR, dir); // set direction in wich the motor turns
  }

  void SetMotorPosition(float angle) // rotates the motor until de potentiometer says that it just reached the angle
  {
    float portion; // variable that affects the speed at wich the motor turns

    if(angle > lastangle) // if the angle needed is higher than the actual angle, I assume that the motor needs to turn RIGHT
    {
      MotorDC::setDIR(RIGHT); // determines the turning Direction
    }
    else
    {
      MotorDC.setDIR(LEFT); // determines the turning Direction
    }

    while(analogRead(POT) != angle)
    {
      portion = 100*(angle - potentiometer.ReadAngle())/360; // calculation of the portion so that when the angle is far it goes faster
      SetMotorSpeed(portion); // it modifies the speed, so that when the angle is close it slows
    }
  }
  
  void SetMotorSpeed(float Speed) // the speed is supposed to come in rmp
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

      for (int i = lastval; i <= val; i = i + BOOST) // it cycles at certain rate, depending on the boost value
      {
        analogWrite(PWM, i); //accelerates "slowly"
        Serial.print("accelarating ");
        Serial.println(i);
        delay(SpeedRegulation(abs)); // the delay varies depending of the increase
      }
    }
    else
    {
      int abs = lastval - val; // determines the increase
      for (int i = lastval; i >= val; i = i - BOOST) // it cycles at certain rate, depending on the boost value
      {
        if(i < 0) // if the value is negative it prevents the motor from having an error
          i = 0;
        analogWrite(PWM, i); //decelerates "slowly"
        Serial.print("decelarating ");
        Serial.println(i);
        if(val != 0) // if the value is 0 then it isn't suppoused to makes any delay
          delay(SpeedRegulation(abs)); // the delay varies depending of the increase
        else
          i++;
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
