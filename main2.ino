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

#define IN1 15
#define IN2 2
#define IN3 4
#define IN4 6
#define PWMEsq 18
#define PWMDir  19
   
#define TIMEOUT     2500  
#define EMITTER_PIN 2
#define NUM_SENSORS 7

int QTR_THRESHOLD = 2000; //nível da linha branca

int setPoint = 4000; //Número que corresponde a linha estar no sensor do meio

int pid_Kp = 20;
int pid_Ki = 50;
int pid_Kd = 100;

int pid_error;
int pid_last_error;
int pid_integral;
int pid_derivative;
int pid_control;


QTRSensorsRC qtrrc ((unsigned char[]) {A0, A1, A2, A3, A4, A5, A6} , NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
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
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  Serial.println("Robot Inicializado");

  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);

for (int i = 0; i < 250; i++)
{
 qtrrc.calibrate();
 delay(20);
}

}
     
void loop()
{
  // lê os valores do sensor
  int sensor_values[7];
  for (int i = 0; i < 7; i++)
  {
    sensor_values[i] = analogRead(i);
  }
  Serial.print("A0: ");
  Serial.println(analogRead(A0)); 
  delay(250);
  Serial.print("A1: ");
  Serial.println(analogRead(A1)); 
  delay(250);
  Serial.print("A2: ");
  Serial.println(analogRead(A2)); 
  delay(250);
  Serial.print("A3: ");
  Serial.println(analogRead(A3)); 
  delay(250);
  Serial.print("A4: ");
  Serial.println(analogRead(A4)); 
  delay(250);
  Serial.print("A5: ");
  Serial.println(analogRead(A5)); 
  delay(250);
  Serial.print("A6j: ");
  Serial.println(analogRead(A6)); 
  delay(250);
  
  // ajusta a linha
  int position =  line_position(sensor_values);

  // calcula o erro
  pid_error = position - setPoint;

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
    analogWrite(18 + i, motor_speed[i]);
  }

  // imprime os valores para depuração
  //Serial.print("Position: ");
  //Serial.println(position);
  //Serial.print("Motor Speed 0: ");
  //Serial.println(motor_speed[0]);
  //Serial.print("Motor Speed 1: ");
  //Serial.println(motor_speed[1]);

  //Serial.println(sensorValues[NUM_SENSORS]);
}

int line_position(int sensor_values[8])
{
  // inicializa a variável de posição
  int position = 0;

  // percorre os sensores
  for (int i = 0; i < 7; i++)
  {
    // se o sensor estiver acima do limiar
    if (sensor_values[i] > QTR_THRESHOLD) //inverter checagem para procurar linha preta
    {
      // adiciona o peso do sensor à posição
      position = position + (i * 1000);
    }
  }

  // divide a posição pelo número de sensores ativos para obter a média
  position = position / 7;

  // retorna a posição
  return position;
}
