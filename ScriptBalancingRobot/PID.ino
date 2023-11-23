//===> Variáveis
double kp = 20.0;
double kd = 0.9;
double ki = 1.1;

int OUTMAX = 100;
int OUTMIN = -100;

double SetPoint = 6.0;

float lastInput = 0.0;

double ITerm = 0.0;

double Compute(double input){

  // Obtendo sinal de erro
  double erro = SetPoint - input;

  // Obtendo a integral do Erro
  ITerm += (ki * erro);

  // Colocando uma saturação no integrador
  if(ITerm > OUTMAX){
    ITerm = OUTMAX;
  }else if(ITerm <= OUTMIN){
    ITerm = OUTMIN;     
  }

  // Diferença entre a entrada atual e a anterior
  double dInput = input - lastInput;

  double output = kp * erro + ITerm + kd*dInput;

  // Colocando uma saturação no sinal de controle
  if(output > OUTMAX){
    output = OUTMAX;
  }else if(output <= OUTMIN){
    output = OUTMIN;     
  }
  
  lastInput = input;

  return output;  
}
