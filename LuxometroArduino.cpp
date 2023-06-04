// Definir los pines de conexión para el registro de desplazamiento
const int latchPin = 3;  // Pin ST_CP (RCLK) del registro
const int clockPin = 4; // Pin SH_CP (SRCLK) del registro
const int dataPin = 2;  // Pin DS (SER) del registro

// Definir la cantidad total de LEDs
const int numLEDs = 8;
const int fr = 8;

// Crear un arreglo para almacenar los estados de los LEDs
byte leds[numLEDs];

void setup() {
  // Configurar los pines como salidas
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(fr, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Encender los LEDs uno por uno en secuencia
 
  int foto=analogRead(fr);
  Serial.println(foto);
  delay(500);

  if(foto<122){

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
