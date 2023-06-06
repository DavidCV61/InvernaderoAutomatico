
#include "DHT.h"

#define DHTPIN 14

#include <Servo.h>

const int latchPin = 3;  // Pin ST_CP (RCLK) del registro
const int clockPin = 4; // Pin SH_CP (SRCLK) del registro
const int dataPin = 2;  // Pin DS (SER) del registro
bool Abierto=false;


const int sensorMov=7;
const int sensorLLuvia=6;

// Definir la cantidad total de LEDs
const int numLEDs = 64;
const int tempMax=20;

// Crear un arreglo para almacenar los estados de los LEDs
byte leds[numLEDs];

// Definir los pines del sensor ultrasónico
const int trigPin = 12;
const int echoPin = 13;

// Variables para almacenar el tiempo de vuelo del ultrasonido y la distancia
long tiempoVuelo;
int distancia;


const int sensorFuego=8;



Servo miServo;
int angulo = 0;

#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
    
    pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

   pinMode(sensorFuego, INPUT);
  pinMode(sensorMov, INPUT);
  pinMode(sensorLLuvia, INPUT);
   miServo.attach(9); 
   // Inicializar los pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);




  dht.begin();
}

void loop() {
  Serial.println("-----NEVOS DATOS-----");




 
  delay(1000);
  int sensorHumedad= analogRead(A5);
  int sensorAire= analogRead(A4);
  int sensorLuz= analogRead(A3);
  Serial.print("Intensidad de luz: ");
  Serial.println(sensorLuz);

  if(sensorLuz>=500){

    encenderLED(1);
  }
  else{
    apagarLED(1);
  }
  Serial.print("Nivel de CO2:");
  Serial.println(sensorAire);

    int Fuego = digitalRead(sensorFuego);  // Leer el valor del sensor de llama

  if (Fuego != HIGH) {
    
    Serial.println("¡Llama detectada!");  // Imprimir mensaje en la consola
    encenderLED(5);
   
  } else {
   
    Serial.println("Sin llama");  // Imprimir mensaje en la consola
     apagarLED(5);
  }



  // Imprimir el valor de humedad en la consola serial
 Serial.print("Humedad del suelo: ");
  Serial.println(sensorHumedad);
    digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir el tiempo de vuelo del ultrasonido en el pin ECHO
  tiempoVuelo = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  distancia = tiempoVuelo * 0.034 / 2;

  // Mostrar la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
 

  if(sensorHumedad>=1000){
    Serial.println("Sensor desconectado");
  }
  else if(sensorHumedad<1000 && sensorHumedad>=600){
  
    Serial.println("Suelo seco");

    if(distancia>=10){
      apagarLED(2);
      encenderLED(4);

    }else{
      apagarLED(4);
      encenderLED(2);
    }

  }
  else if(sensorHumedad<600 && sensorHumedad>=370){
       Serial.println("Suelo humedo");
       apagarLED(2);
  }
  else if(sensorHumedad<600){
       Serial.println("Suelo es agua");
       apagarLED(2);

  }
 





  
  int lluvia= digitalRead(sensorLLuvia);

 

    if(lluvia ==1){

       Serial.println("No hay lluvia");
 

      if(Abierto){

        servoCerrar();
        Abierto=false;

      }else{

        miServo.write(0);
        
      }
    }else{

      Serial.println("Si hay lluvia");
        if(Abierto){
          miServo.write(0);

        

      }else{

        servoAbrir();
        Abierto=true;
      }

    }
  bool movimiento=digitalRead(sensorMov);
  if(movimiento==true) {

    encenderLED(6);
    apagarLED(7);
    Serial.println("Se mueve");


  }else{
    encenderLED(7);
    apagarLED(6);
    Serial.println("No se mueve");
  }
  



 
  float h = dht.readHumidity();
 
  float t = dht.readTemperature();


  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    
  }else{

     Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  }



 
  if(t>=tempMax){

     encenderLED(0);
  }else{
     apagarLED(0);
  }
   
 
}
// Función para encender un LED específico
void encenderLED(int ledIndex) {
  // Verificar el índice del LED
  if (ledIndex < 0 || ledIndex >= numLEDs) {
    return; // Salir si el índice está fuera de rango
  }
  
  // Encender el LED correspondiente en el arreglo
  leds[ledIndex] = HIGH;

  // Actualizar los registros de desplazamiento
  actualizarRegistros();
}

// Función para apagar un LED específico
void apagarLED(int ledIndex) {
  // Verificar el índice del LED
  if (ledIndex < 0 || ledIndex >= numLEDs) {
    return; // Salir si el índice está fuera de rango
  }
  
  // Apagar el LED correspondiente en el arreglo
  leds[ledIndex] = LOW;

  // Actualizar los registros de desplazamiento
  actualizarRegistros();
}

// Función para actualizar los registros de desplazamiento
void actualizarRegistros() {
  // Desactivar la salida de los registros de desplazamiento
  digitalWrite(latchPin, LOW);

  // Enviar los datos de los LEDs a los registros de desplazamiento
  for (int i = numLEDs - 1; i >= 0; i--) {
    // Desplazar el bit correspondiente al LED actual
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, leds[i]);
    digitalWrite(clockPin, HIGH);
  }

  // Activar la salida de los registros de desplazamiento
  digitalWrite(latchPin, HIGH);
}

void servoAbrir(){
 for (angulo = 0; angulo <= 90; angulo += 1) {
    miServo.write(angulo);
    
    delay(15);  // Pequeña pausa para que el servo se mueva
  }
  
  }

  void servoCerrar(){
 

  for (angulo = 90; angulo >= 0; angulo -= 1) {
    miServo.write(angulo);
    delay(15);
  }
  
}
