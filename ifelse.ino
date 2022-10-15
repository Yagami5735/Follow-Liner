#include <analogWrite.h>

#define A0 14
#define A1 27
#define A2 26
#define A3 25

#define IN1 15
#define IN2 2
#define IN3 4
#define IN4 5
#define PWMEsq 18
#define PWMDir  19

#define velocMotorEsq 200
#define VelocMotorDir 200

#define INN 33

int i = 0;

void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    
    pinMode(PWMEsq, OUTPUT);
    pinMode(PWMDir, OUTPUT);

    pinMode(INN, INPUT);
    
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    
    digitalWrite(35, HIGH);
    digitalWrite(34, HIGH);
    
}

void loop() {

  Serial.println("-- start --");          
  delay(8000);
  
  while(true)
  {
    /*Serial.print("A0: ");
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
    delay(250);*/
    
    //Serial.print("INN: ");
    //Serial.println(analogRead(INN));
    
    if(analogRead(A1) < 200 && analogRead(A2) < 200 && analogRead(A0) > 700  && analogRead(A3) > 700)
    {
      motors(200, 200);
      Serial.println("Frente");
    }
    
    else if(analogRead(A0) < 200 && analogRead(A1) < 200 && analogRead(A2) < 200 && analogRead(A3) < 200)
    {
      motors(200, 200);
      Serial.println("Cruzamento"); 
    }
    
    else if(analogRead(A0) > 700 && analogRead(A1) > 700 && analogRead(A2) < 200 && analogRead(A3) < 200)
    {
      motors(50, 200); 
      Serial.println("Esquerda média");
    }

    else if(analogRead(A0) < 200 && analogRead(A1) < 200 && analogRead(A2) > 700 && analogRead(A3) > 700)
    { 
      motors(200, 50);
      Serial.println("Direita média");
    }

    else if(analogRead(A0) > 700 && analogRead(A1) > 700 && analogRead(A2) > 700 && analogRead(A3) < 200)
    {
      motors(0, 200); 
      Serial.println("Esquerda média");
    }

    else if(analogRead(A0) < 200 && analogRead(A1) > 700 && analogRead(A2) > 700 && analogRead(A3) > 700)
    { 
      motors(200, 0);
      Serial.println("Direita média");
    }

    else if(analogRead(A0) > 700 && analogRead(A1) < 200 && analogRead(A2) < 200 && analogRead(A3) < 200)
    {
      motors(185, 200);
      Serial.println("Esquerda coisa poka");  
    }

    else if(analogRead(A0) < 200 && analogRead(A1) < 200 && analogRead(A2) < 200 && analogRead(A3) > 700)
    {
      motors(200, 185);;  
      Serial.println("Direita coisa poka");  
    }
    
    if(analogRead(INN) < 300)
    {
       Serial.println("i++");
       i++;
       Serial.println(i);
       delay(250);
    }
    
    if(i == 2)
    {
       Serial.println("break");
       break;
    }
  }
  
  motors(0, 0);
  Serial.println("Paro!");
  delay(60000);
}

void motors(int esq, int dir){
  analogWrite(PWMEsq, esq);
  analogWrite(PWMDir, dir); 
}
