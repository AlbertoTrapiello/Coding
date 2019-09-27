#define MAX_CHECK 4

void setup()
{
  // put your setup code here, to run once:
  pinMode(A0, INPUT); // A0 is the analogRead input for the potentiometer
  pinMode(3, INPUT); // it would makes as if its the direction variable
  Serial.begin(9600);
}
//SOLUCIÓN PARA LA ZONA DE RUIDO QUIZÁ DECIDOR QUE SI LLEGA A 340 Y NO HA CAMBIADO EL SENTIDO NO HACER CASO HASTA QUE TOME UN 0
//O EVALUAR SI LOS PUNTOS QUE LEE EN ALGÚN MOMENTO VAN EN CONTRA DEL SENTIDO QUE LLEVA (SI ESTÁ SUBIENDO Y LLEGA A 1023 Y VE QUE BAJA) LO IGNORA

enum Direction {RIGHT = 1, LEFT = 0};

int input; // analog read
int lastInput; // Last analog Read
Direction DIR; // Direction

bool CheckNoise(Direction Dir = RIGHT) // Function responsible to avoid the fake values
{
  bool output[MAX_CHECK];
  int error = 0;

  for(int i = 0; i < MAX_CHECK; i++)
  {
    input = analogRead(A0);
    Serial.println("**********************");
    Serial.println(i);
    Serial.print("INPUT: ");
    Serial.println(input); // prints the value of the analogRead
    Serial.print("LAST INPUT: ");
    Serial.println(lastInput); // prints the value of the analogRead
    Serial.println("**********************");
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
    Serial.print("Output[i]: ");
    Serial.println(output[i]);
  }
  for(int i = 0; i< MAX_CHECK; i++)
  {
    if(output[i] == false)
      error++;
  }

  Serial.print("ERROR = ");
  Serial.println(error);
  if(error > 0 )
    return false;
  else
    return true;
}

void loop()
{
  // put your main code here, to run repeatedly:

  //input = analogRead(A0); // Saves the value read
  DIR = digitalRead(3); // Saves the DIR variable

  Serial.print("Direction => ");
  Serial.println(DIR);

  if(CheckNoise(DIR))
  {
    Serial.println("--------------------------");
    Serial.print("ANALOG: ");
    Serial.println(analogRead(A0)); // prints the value of the analogRead
    Serial.print("ANGLE: ");
    Serial.println(map(analogRead(A0), 0, 1023, 0, 340)); // print the angle it suppoused to be  */
  }
  else
  {
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.println();
    Serial.println("Not a good meassure");
    Serial.println();
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
  }
  lastInput = input; // Saves the last input
  Serial.println("--------------------------");
  //delay(500);
}
