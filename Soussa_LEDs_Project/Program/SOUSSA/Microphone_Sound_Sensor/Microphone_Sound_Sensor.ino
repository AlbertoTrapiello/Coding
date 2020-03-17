
void setup() 
{
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  Serial.print("Reading: ");
  double a = analogRead(A0);
  Serial.println(a);

}
