int DIR = 2, PWM = 3;
class MotorDC
{
  int DIR; //pin asociado al sentido de giro
  int PWM; //pin asociado a la entrada

public:
  MotorDC()
  {
    DIR = 2;
    PWM = 3;
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

  for (int value = 0; value < 100; value++)
  {
    analogWrite(PWM, map(value, 0, 100, 0, 255));
    Serial.println(map(value, 0, 100, 0, 255));
    delay(50);
  }

   digitalWrite(DIR, LOW);

  for (int value = 0; value < 100; value++)
  {
    analogWrite(PWM, map(value, 0, 100, 0, 255));
    Serial.println(map(value, 0, 100, 0, 255));
    delay(50);
  }
}
