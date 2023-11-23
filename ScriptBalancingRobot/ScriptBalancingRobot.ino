/* ========================================================
 *  =========== SCRIPT DO ROBÔ EQUILIBRISTA ===============
 *  =======================================================
*/
#include <Wire.h>
#include <Kalman.h>

//Variáveis
uint8_t i2c_data[14];
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;

uint32_t timer;

//Filtro de Kalman
Kalman KalmanX;
Kalman KalmanY;
Kalman KalmanZ;

double KalAngleX;
double KalAngleY;
double KalAngleZ;

double gyroXangle;
double gyroYangle;

void setup() {

  // Inicializando a Serial para exibir a mensagem de Debug
  Serial.begin(115200);

  // Iniciando o Barramento i2c para comunicar com o sensor MPU6050
  Wire.begin();

// Setando a frequencia correta de operação
#if ARDUINO >= 157
  Wire.setClock(400000UL); // Freq = 400kHz
#else
  TWBR = ((F_CPU/400000UL) - 16)/2;  // Freq = 400kHz
#endif

  i2c_data[0] = 7;
  i2c_data[1] = 0x00;
  i2c_data[2] = 0x00;
  i2c_data[3] = 0x00;

  // Configuração do i2C
  while(i2cWrite(0x19, i2c_data, 4, false));
  while(i2cWrite(0x6B, 0x01, true));
  
  while(i2cRead(0x75, i2c_data, 1));

  //Verificação da conexão da placa
  if(i2c_data[0] != 0x68){
    //Não é a placa MPU6050 conectada
    Serial.print("ERRO: Placa desconhecida\n");
    while(1){
    Serial.print("ERRO: Conecte a MPU6050 no barramento i2c\n");
    }
  }
  
  //Tempo para estabilizar o sensor
  delay(100);
  
  /* 1 - Leitura dos dados de Acc XYZ */
  while(i2cRead(0x3B, i2c_data,14));

 /* 2 - Organizar os dados de Acc XYZ */
  accX = (int16_t)((i2c_data[0] << 8) | i2c_data[1]); // ([ MSB ] [ LSB ])
  accY = (int16_t)((i2c_data[2] << 8) | i2c_data[3]); // ([ MSB ] [ LSB ])
  accZ = (int16_t)((i2c_data[4] << 8) | i2c_data[5]); // ([ MSB ] [ LSB ])

  // Calculo do Pitch e Roll
  double pitch = atan(accX/sqrt(accY*accY + accZ*accZ))*RAD_TO_DEG;
  double roll = atan(accY/sqrt(accX*accX + accZ*accZ))*RAD_TO_DEG;

  // Utilização do Filtro de Kalman
  KalmanX.setAngle(roll);
  KalmanY.setAngle(pitch);

  gyroXangle = roll;
  gyroYangle = pitch;  
 
  timer = micros();

  //Iniciando os motores
  int_motores();
  //Serial.print("Fim do Setup\n");
}

void loop() {

  while(i2cRead(0x3B, i2c_data,14));

  accX = (int16_t)((i2c_data[0] << 8) | i2c_data[1]);   //([MSB] [LSB])
  accY = (int16_t)((i2c_data[2] << 8) | i2c_data[3]);   //([MSB] [LSB])
  accZ = (int16_t)((i2c_data[4] << 8) | i2c_data[5]);   //([MSB] [LSB])

  //gyroX = (int16_t)((i2c_data[8] << 8) | i2c_data[9]);   //([MSB] [LSB])
  gyroY = (int16_t)((i2c_data[10] << 8) | i2c_data[11]);   //([MSB] [LSB])
  //gyroZ = (int16_t)((i2c_data[12] << 8) | i2c_data[13]);   //([MSB] [LSB])

// LEITURAS PARA TESTAR O SENSOR MPU6050
  //Serial.print("GiroXYZ");  Serial.print("\n");
  //Serial.print(accX);  Serial.print("\n");
  //Serial.print(accY);  Serial.print("\n");
  //Serial.print(accZ);  Serial.print("\n");
  //Serial.print("GiroXYZ");  Serial.print("\n");
  //Serial.print(gyroX);  Serial.print("\n");
  //Serial.print(gyroY);  Serial.print("\n");
  //Serial.print(gyroZ);  Serial.print("\n");
  
  //Serial.print(roll);  Serial.print("\n");
  //Serial.print(pitch);  Serial.print("\t");

  //Calculando a parcela de tempo que demora para obter os dados do acelerometro e giroscópio
  double dt = (double)(micros() - timer)/1000000;
  timer = micros();
  
  double pitch = atan(accX/sqrt(accY*accY + accZ*accZ))*RAD_TO_DEG;
  //double roll = atan(accY/sqrt(accX*accX + accZ*accZ))*RAD_TO_DEG;

  //gyroXangle = gyroX / 131.0; // graus/segundo
  gyroYangle = gyroY / 131.0; // graus/segundo
  
  //KalAngleX = KalmanX.getAngle(roll, gyroXangle, dt);
  KalAngleY = KalmanY.getAngle(pitch, gyroYangle, dt);

  // Testando a estimação do Filtro de Kalman
  //Serial.print(pitch);  Serial.print("\t");
  //Serial.print(KalAngleY);  Serial.print("\n");

  //Atuando nos Motores  
  double SinalDeControle = Compute(KalAngleY);
  PWMControleMotores(SinalDeControle);
}
