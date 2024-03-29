#include <analogWrite.h>

#include <QTRSensors.h>

#define A0 13
#define A1 12
#define A2 14
#define A3 27
#define A4 26
#define A5 25
#define A6 33
#define A7 32

#define IN1 13
#define IN2 2
#define IN3 4
#define IN4 6
#define PWMEsq 16
#define PWMDir 19
   
#define TIMEOUT     2500  
#define EMITTER_PIN 2
#define NUM_SENSORS 8

int QTR_THRESHOLD = 2000;

int pid_Kp = 20;
int pid_Ki = 50;
int pid_Kd = 100;

int pid_error;
int pid_last_error;
int pid_integral;
int pid_derivative;
int pid_control;


QTRSensorsRC qtrrc ((unsigned char[]) {A0, A1, A2, A3, A4, A5, A6, A7} , NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];



void setup()
{
  // inicializa o robô
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.println("Robot Inicializado");

for (int i = 0; i < 250; i++)
{
 qtrrc.calibrate();
 delay(20);
}
}

void loop()
{
  // lê os valores do sensor
  int sensor_values[8];
  for (int i = 0; i < 8; i++)
  {
    sensor_values[i] = analogRead(i);
  }

  // ajusta a linha
  int position =  line_position(sensor_values);

  // calcula o erro
  pid_error = position - QTR_THRESHOLD;

  // calcula o integral
  pid_integral = pid_integral + pid_error;

  // calcula o derivativo
  pid_derivative = pid_error - pid_last_error;

  // calcula o pid_control
  pid_control = (pid_Kp * pid_error) + (pid_Ki * pid_integral) + (pid_Kd * pid_derivative);

  // atualiza os valores para o próximo cálculo
  pid_last_error = pid_error;

  // ajusta a velocidade dos motores
  int motor_speed[2];
  motor_speed[0] = 100 - pid_control;
  motor_speed[1] = 100 + pid_control;

  // atualiza os motores
  for (int i = 0; i < 2; i++)
  {
    analogWrite(6 + i, motor_speed[i]);
  }

  // imprime os valores para depuração
  Serial.print("Position: ");
  Serial.println(position);
  Serial.print("Motor Speed 0: ");
  Serial.println(motor_speed[0]);
  Serial.print("Motor Speed 1: ");
  Serial.println(motor_speed[1]);

  Serial.println(sensorValues[NUM_SENSORS]);
}

int line_position(int sensor_values[8])
{
  // inicializa a variável de posição
  int position = 0;

  // percorre os sensores
  for (int i = 0; i < 8; i++)
  {
    // se o sensor estiver acima do limiar
    if (sensor_values[i] > QTR_THRESHOLD)
    {
      // adiciona o peso do sensor à posição
      position = position + (i * 1000);
    }
  }

  // divide a posição pelo número de sensores ativos para obter a média
  position = position / 8;

  // retorna a posição
  return position;
}
