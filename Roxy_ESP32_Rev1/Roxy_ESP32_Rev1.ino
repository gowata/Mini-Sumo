/*-------------------------------------------------------------------------
 * Título: Programa placa Roxy para Mini Sumô ESP32
 * Data: 04.01.2025
 * Arquivo: Roxy_ESP32_Rev1.ino
 * Autor: Rodrigo Horikawa Watanabe
 * Rev: 1
 * 
 * Hardware: Esp32 30 pinos
 * Descrição:
 * Programa para rodar o Robô Mini Sumo com ESP32 
 * - Controle do Driver L293D
 * - Controle de Bateria 
 * - Indicador de Conexão de Bluetooth
 * 
%------------------------------------------------------------------------*/

#include <BluetoothSerial.h>

//-------- Inicialização do Bluetooth -------------------------------------------------------------
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

/* 
Configuração de Eventos

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif
*/

//---- Configuração dos pinos do Driver

#define IN1 33 
#define IN2 25
#define IN3 18
#define IN4 19 
#define EN1 32
#define EN2 5

//----- Configurando os leds ----------------------------------------------------------------------

#define ledBateria    12    //Led bateria 
#define ledBluetooth  13    //Led Bluetooth 

//----- Configurando o Medidor de Bateria

#define medBateria    14    

//Conversor AD conectado no pino 7 - GPIO32 -  ADC4
//Conversor AD tipo SAR 12bits
//ADC1
//ADC - RTC
//ADC - DMA
//ADC Attenuation

//--------------------------------------

BluetoothSerial SerialBT;

char val; // variable to receive data from the serial port

int AN_Bat_Result = 0;  //Inicializando valor de Leitura
float Voltage = 0.0;

/* BT: Bt_Status callback function
void Bt_Status(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) 
{
  if (event == ESP_SPP_OPEN_EVT) 
  {                                                        // BT: Checks if the SPP connection is open, the event comes// event == Client connected
    digitalWrite (ledBluetooth, HIGH);                                                     // BT: Turn ON the BLUE bluetooth indication LED (solid light)
  }
  else if (event == ESP_SPP_CLOSE_EVT)
  {                                                  // BT: event == Client disconnected
    digitalWrite(ledBluetooth, LOW);                                                       // BT: Turn OFF the BLUE bluetooth indication LED
  }
}
*/

void setup() {

  Serial.begin(115200);
  SerialBT.begin("Japas"); 
//  Serial.println("O dispositivo já pode ser pareado!");

  adcAttachPin(medBateria);
  analogSetClockDiv(255);
  analogSetWidth(12);
  analogSetAttenuation(ADC_11db);
  analogReadResolution(12);
  
  pinMode(ledBateria,OUTPUT);
  pinMode(ledBluetooth,OUTPUT); 
  
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);  
  pinMode(IN3, OUTPUT);  
  pinMode(IN4, OUTPUT);  
  pinMode(EN1, OUTPUT);  
  pinMode(EN2, OUTPUT);  
    
//---- Pisca 3x o Led do Esp32

  digitalWrite(ledBateria, HIGH);  
  delay(200);
  digitalWrite(ledBateria, LOW);  
  delay(200);
  digitalWrite(ledBateria, HIGH);  
  delay(200);
  digitalWrite(ledBateria, LOW);  
  delay(200);
  digitalWrite(ledBateria, HIGH);  
  delay(200);
  digitalWrite(ledBateria, LOW);  
//--------------------------------------

  digitalWrite(EN1, HIGH);  // turn ON Motor 1
  digitalWrite(EN2, HIGH);  // turn ON Motor 2

//SerialBT.register_callback(Bt_Status);  // BT: Define o tratamento de evento Bt_Status

}

void loop() {

  AN_Bat_Result = analogRead(medBateria);

  Voltage = AN_Bat_Result;
  /*
  Serial.print("AD: ");
  Serial.println(AN_Bat_Result);
  Serial.print("Voltage: ");
  Serial.println(Voltage/1000);
*/


  if(AN_Bat_Result<2000)    // Nas medidas fica perto de 9,2V
  {
    digitalWrite(ledBateria, HIGH);    
  }
  else
  {
    digitalWrite(ledBateria, LOW);   
  }

  if (SerialBT.available()) 
  {
    val =(char)SerialBT.read();
    digitalWrite (ledBluetooth, HIGH);
  }
  
 
  if( val == 'F' )           // Frente
  {
    digitalWrite(IN1, HIGH);   // otherwise turn it OFF
    digitalWrite(IN2, LOW);   // otherwise turn it OFF
    digitalWrite(IN3, HIGH);   // otherwise turn it OFF
    digitalWrite(IN4, LOW);   // otherwise turn it OFF
  } 
  if( val == 'B' )            // Tras
  {
    digitalWrite(IN1, LOW);   // otherwise turn it OFF
    digitalWrite(IN2, HIGH);   // otherwise turn it OFF
    digitalWrite(IN3, LOW);   // otherwise turn it OFF
    digitalWrite(IN4, HIGH);   // otherwise turn it OFF
  } 
  if(val == 'L')              //Esquerda
  {
    digitalWrite(IN1, HIGH);   // otherwise turn it OFF
    digitalWrite(IN2, LOW);   // otherwise turn it OFF
    digitalWrite(IN3, LOW);   // otherwise turn it OFF
    digitalWrite(IN4, HIGH);   // otherwise turn it OFF
  }

  if(val == 'R')              //Direita
  {
    digitalWrite(IN1, LOW);   // otherwise turn it OFF
    digitalWrite(IN2, HIGH);   // otherwise turn it OFF
    digitalWrite(IN3, HIGH);   // otherwise turn it OFF
    digitalWrite(IN4, LOW);   // otherwise turn it OFF
  }
  if(val == 'S')              //Stop
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2, LOW);   // otherwise turn it OFF
    digitalWrite(IN3, LOW);   // otherwise turn it OFF
    digitalWrite(IN4,LOW);
  }
  if(val == 'G')              //Frente Esquerda
  {
    digitalWrite(IN1, HIGH);   // otherwise turn it OFF
    digitalWrite(IN2, LOW);   // otherwise turn it OFF
    digitalWrite(IN3, LOW);   // otherwise turn it OFF
    digitalWrite(IN4, LOW);   // otherwise turn it OFF
  }
  if(val == 'I')              //Frente Direita
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2, LOW);   // otherwise turn it OFF
    digitalWrite(IN3, HIGH);   // otherwise turn it OFF
    digitalWrite(IN4,LOW);
  }
  if(val == 'H')              //Tras Esquerda
  {
    digitalWrite(IN1, LOW);   // otherwise turn it OFF
    digitalWrite(IN2, HIGH);   // otherwise turn it OFF
    digitalWrite(IN3, LOW);   // otherwise turn it OFF
    digitalWrite(IN4, LOW);   // otherwise turn it OFF
  }
  if(val == 'J')              //Tras Direita
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2, LOW);   // otherwise turn it OFF
    digitalWrite(IN3, LOW);   // otherwise turn it OFF
    digitalWrite(IN4,HIGH);
  }

  delay(10);                    // wait 100ms for next reading
  digitalWrite (ledBluetooth, LOW);


}
