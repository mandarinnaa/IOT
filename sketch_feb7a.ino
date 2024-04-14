int Valor;
int D2=D2;
int D13=D13;
void setup() {
  // put your setup code here, to run once:
  pinMode(D2,INPUT);

  pinMode(D13,OUTPUT);
  Seria1.begin(11500);
}

void loop() {
  // put your main code here, to run repeatedly:
Valor=analogRead(0);
if(Valor>125){
  digitalWrite(D13,HIGH);
}else{
  digitalWrite(D13,LOW);
}
Serial1.println(Valor);
delay(300);
}