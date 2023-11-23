
const int OUTA = 3;
const int OUTB = 9;
const int OUTC = 5;
const int OUTD = 11;

void int_motores(){
  pinMode(OUTA, OUTPUT);
  pinMode(OUTB, OUTPUT);
  pinMode(OUTC, OUTPUT);
  pinMode(OUTD, OUTPUT); 
}

void PWMControleMotores(double comando){

  if(comando > 10){
    analogWrite(OUTA, 0); // Motor da direita p/ trás
    analogWrite(OUTB, abs(comando)); // Motor da direita p/ frente
    analogWrite(OUTC, 0); // Motor da esquerda p/ trás
    analogWrite(OUTD, abs(comando)); // Motor da esquerda p/ frente
  }else{
    analogWrite(OUTA, abs(comando)); // Motor da direita p/ trás
    analogWrite(OUTB, 0); // Motor da direita p/ frente
    analogWrite(OUTC, abs(comando)); // Motor da esquerda p/ trás
    analogWrite(OUTD, 0); // Motor da esquerda p/ frente
  }
}
