#include <Wire.h> //librería de gestión del I2C (reduce el nº de pines a 4 GND,VCC, SCL(pin A5) y SDA(pin A4)
#include <LiquidCrystal_I2C.h> //librería específica de gestión del display con I2C

/* Autor: Andoni Alonso
   Fecha: Diciembre 12, 2017
   Titulo: Activar "Relay-5V" con lámpara UV cuando se detecta actividad de las bombas del estanque, mostrar en el LCD 16x2 información de 
   -situación de la salida 
      0123456789012345
     0I7.28 hh:mm:ss O
     1___--ðð__--_____
   Descripcion: 
   Conexión del componente al Arduino Uno:
    S: pin 2
    +: 5V
    -: GND
*/

// Definir
int relayPin = 2;
int currentSensor = 0; // Analog input 0
int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A Module
int RawValue = 0; // Input Value
double currentThreshold = 0.7; // Umbral de sensibilidad a la corriente en miliamperios (0.7 para el panel solar y 0.3 para la linterna de petaca.
int ACSoffset = 2500;
double Voltage = 0;
double Amps = 0;

//Conteo del tiempo de actividad
unsigned long time;

//Situación de actividad del relé, en alto es que hay entrada para activar la salida (aunque no se active porque ponemos un periodo refractario de 60000 milisegundos).
boolean relayStatus = false;

//Situación de actividad de la bomba
boolean pumpStatus = false;

//Objeto LCD visor de resultados
//int lcdColumns = 16;
//int lcdLines = 2;
int lcdColumns = 20;
int lcdLines = 4;
LiquidCrystal_I2C lcd (0x27, lcdColumns, lcdLines); //0x27=dirección del lcd, 16=nº de columnas, 2=nº de filas

void setup() {
 
  // Configuración
  pinMode(relayPin, OUTPUT); // Configurar relay como salida o OUTPUT
  Serial.begin(9600); // Abrir el puerto serie a la velocidad de 9600bps para transmisión de datos.

  Wire.begin(); //Inicializar I2C
  lcd.begin(lcdColumns, lcdLines); //Inicializar I2C
  lcd.clear(); // borrar pantalla
  lcd.backlight(); //activar la luz de fondo (nobacklight() sería para apagar) TODO:decidir el mejor modo en función del resultado in situ
}

void loop() {

  // read the sensor's input
  RawValue = analogRead(currentSensor);
  Voltage = (RawValue * 4.8828125); // Gets you mV 4.8828... = 5000/1024.0; 
  Amps = ((Voltage - ACSoffset) / mVperAmp);

  lcdDisplayAmps(Amps); // mostramos el valor en el LCD
  Serial.print("Amps: ");
  Serial.println(Amps); 
  

  Serial.print("mVolts: ");
  Serial.println(Voltage); 

  Serial.print("RawValue: ");
  Serial.println(RawValue); 

  
  if (abs(Amps) >= currentThreshold) 
  {
    //si el estado del relé era apagado, al tener el sensor activo empezamos a contar el tiempo
    if (!relayStatus)
    {
      time = millis();
      relayStatus = true;
      Serial.print("Empiezo a contar");
    }
    else
    {
      if (millis() - time > 60000 )
      {
        // Accionar el relay
        digitalWrite(relayPin, HIGH); // envia señal alta al relay
        pumpStatus = true;

        Serial.print("Tiempo de espera ha llegado ");
        //Mostrar señal de salida en el LCD
        lcd.setCursor(15, 1);
        lcd.print("O");
        }
    }
  }
  else
  {
    digitalWrite(relayPin, LOW);  // envia señal baja al relay
    //Serial.println("Relay no accionado");
    time = 0;
    relayStatus = false;
    pumpStatus = false;
    
    //Mostrar señal de salida en el LCD
    lcd.setCursor(15, 1);
    lcd.print("-");

  }

  lcdDisplayTime(); //mostrar la información de tiempo de actividad.
  delay(1000);           // 1 segundo

}

//Mostrar la lectura de la intensidad actual
void lcdDisplayAmps(double Amps){

  //??char AmpsString[5];  //  Hold The Convert Data
  //??dtostrf(Amps,2,2,AmpsString); // dtostrf( [doubleVar] , [sizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )

  lcd.setCursor(0,1);
  lcd.print("I:");
  //??lcd.print(String(AmpsString));
  lcd.print(Amps,2);

}

//Mostrar la información de tiempo de actividad
void lcdDisplayTime(){
  }

