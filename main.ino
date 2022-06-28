#include <QTRSensors.h>
int motorEsquerda = 23;
int motorDireita = 22;
float KP = 0.3;
float KD = 7;
int erroAnterior = 0;
int velocidadeMotor1 = 255;
int velocidadeMotor2 = 255;
#define NUM_SENSORS 8
#define TIMEOUT     2500  
#define EMITTER_PIN 2     


QTRSensorsRC qtrrc ((unsigned char[]) {0, 1, 2, 3, 4, 5, 6, 7} , NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

void setup()
{
 pinMode(motorEsquerda,OUTPUT);
 pinMode(motorDireita,OUTPUT);
 analogWrite(motorEsquerda,127);
 analogWrite(motorDireita,127);
 delay(2000);

 analogWrite(motorEsquerda,175);
 analogWrite(motorDireita,50);
 // CALIBRAÇÃO DOS SENSORES - 5 SEGUNDOS 47
 int i;
 for (i = 0; i < 250; i++)
 {
 qtrrc.calibrate();
 delay(20);
 }
 analogWrite(motorEsquerda,127);
 analogWrite(motorDireita,127);
 delay(2000);
}
//////////////////////// FIM DO SETUP
//////////////////////
void loop()
{
 // LEITURA DOS SENSORES
unsigned int sensors[8];
 int position = qtrrc.readLine(sensors,QTR_EMITTERS_ON,1);
/////////  CALCULO PID //////////
 int erro = position - 3500;
 int sinalControlador = KP * erro + KD * (erro -
erroAnterior);
 erroAnterior = erro;
 int novaVelocidadeMotor1 = velocidadeMotor1 +
sinalControlador;
 int novaVelocidadeMotor2 = velocidadeMotor2 -
sinalControlador;

//// CORREÇÃO DOS VALORES EXTREMOS DOS MOTORES 
 if (novaVelocidadeMotor1 < 127)
 novaVelocidadeMotor1 = 127;
 if (novaVelocidadeMotor2 < 127)
 novaVelocidadeMotor2 = 127;
 if (novaVelocidadeMotor1 > 255)
 novaVelocidadeMotor1 = 255;
 if (novaVelocidadeMotor2 > 255)
 novaVelocidadeMotor2 = 255;
 // APLICAÇÃO DAS NOVAS VELOCIDADES DOS MOTORES VIA PWM
 analogWrite(motorEsquerda,novaVelocidadeMotor1);
 analogWrite(motorDireita,novaVelocidadeMotor2);
} // FIM DO VOID LOOP
