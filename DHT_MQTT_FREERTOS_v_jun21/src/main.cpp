/*********************************************************
* Exemplo para assinatura e publicação em  tópico MQTT, leitura de teclado e exibição lcd
* Por: Fábio Souza
* Adaptação: Marcos Chaves junho/2021
*********************************************************/

/*
Task             Core  Prio     Descrição
-------------------------------------------------------------------------------
vTaskSensor        1     1     faz a leitura do sensor analógico
vTaskPrint         1     1     Imprime o valor do sensor o display
vTaskMQTT          0     2     Publica valor do Sensor em tópico MQTT
vTaskTeclado       0     3     Faz a leitura da tecla e aciona saida

Timer RTOS
---------------------------------------------------------
  xTimer    a cada 2 segundos alterna um Led
  xTimerDHT  a cada x segundos lê sensor DHT e publica em tópico
*/


/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


/*Bibliotecas para o display*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// LCD display connected to I2C (SDA, SCL pins)
LiquidCrystal_I2C lcd(0x3F, 16, 2);


#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


#define DHTPIN 26     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

/*bibliotecas para MQTT*/
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

/*definição dos pinos*/
#define SENSOR_PIN 34
#define BT1 12
#define LED_HEART_BEAT 2
#define OUTPUT_1 27
#define OUTPUT_2 14


#define PRESSIONADO LOW
#define SOLTO   HIGH

/* instacias para WIFI e client*/
WiFiClient espClient;
PubSubClient client(espClient);

/* configuraçãoes da REDE e broker MQTT*/
const char* ssid = "redewifi";
const char* password =  "senha";
const char* mqttServer = "url_brocker";
const int mqttPort = 1883;
const char* mqttUser = "login";
const char* mqttPassword = "senha";

xTimerHandle xTimer, xTimerDHT;
QueueHandle_t xFila; /*cria objeto fila */

/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTaskPrintHandle;
TaskHandle_t xTaskADCHandle;
TaskHandle_t xTaskSensorHandle;
TaskHandle_t xTaskMQTTHandle;
TaskHandle_t xTaskTecladoHandle;

/* Protótipo das Tasks*/
void vTaskSensor(void *pvParameters ); 
void vTaskPrint(void *pvParameters);
void vTaskMQTT(void *pvParameters); 
void vTaskTeclado( void *pvParameters );

/* timer rtos */
void callBackTimer(TimerHandle_t pxTimer );
void callBackTimerDHT(TimerHandle_t pxTimerDHT);

/* Funções auxiliares */
void initSaida();
void rtosInit();
void mqttInit();
int sensorRead();
void initTeclado(int pin);
bool leTeclado(unsigned char tecla);
void displayInit();
void imprimeSensorDisplay(int valor);
void initSaida();
void mqttSendJson(float valor1);
void mqttDHTSendJson();
void mqttSendJsonIO(void);
void mqttSendJsonIOviatask(void);

///  S E T U P 
void setup() {

  initSaida();
  mqttInit(); 
  displayInit(); 
  rtosInit();
}

// L O O P
void loop() {
    //Uma vez conectado client.loop() deve ser chamada periodicamente para manter conexão
    //e aguardar recebimento de mensagens
  client.loop(); //atualiza conexão MQTT 
  vTaskDelay(pdMS_TO_TICKS(1000));    /* Delay de 1 segundos */
}

/* Função Init Harware */
void initSaida(){
    Serial.begin(115200); 
    pinMode(LED_HEART_BEAT,OUTPUT);
    pinMode(OUTPUT_2,OUTPUT);
    pinMode(OUTPUT_1,OUTPUT);     
    dht.begin();
}

void mqttInit(){
   
   WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi..");
   }
  Serial.println("Conectado na rede WiFi");
}

void displayInit(){

	lcd.begin();
	// Turn on the blacklight and print a message.
	lcd.backlight(); 
}


void rtosInit(){

    xFila = xQueueCreate(1, sizeof(int));
if (xFila == NULL)
  {
     Serial.println("Erro: nao e possivel criar a fila");
     while(1); /* Sem a fila o funcionamento esta comprometido. Nada mais deve ser feito. */
  } 
    
    xTimer = xTimerCreate("TIMER",pdMS_TO_TICKS(2000),pdTRUE, 0, callBackTimer);
    xTimerDHT = xTimerCreate("TIMER2",pdMS_TO_TICKS(10000),pdTRUE, 0, callBackTimerDHT);

    xTaskCreatePinnedToCore(
      vTaskSensor,                       /* Funcao a qual esta implementado o que a tarefa deve fazer */
       "TaskADC",                        /* Nome (para fins de debug, se necessário) */
       configMINIMAL_STACK_SIZE + 1024,  /* Tamanho da stack (em words) reservada para essa tarefa */
       NULL,                             /* Parametros passados (nesse caso, não há) */
         2,                              /* Prioridade */
           &xTaskSensorHandle,          /* Handle da tarefa, opcional (nesse caso, não há) */
           APP_CPU_NUM);                /* Core */

    xTaskCreatePinnedToCore(vTaskPrint,  "TaskPrint",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskPrintHandle,APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTaskMQTT,  "TaskMQTT",  configMINIMAL_STACK_SIZE + 2024,  NULL,  3,  &xTaskMQTTHandle,PRO_CPU_NUM);  
    xTaskCreatePinnedToCore(vTaskTeclado,  "Taskteclado",  configMINIMAL_STACK_SIZE+1024 ,  (void *)BT1,  4,  &xTaskTecladoHandle,PRO_CPU_NUM);   

    xTimerStart(xTimer,0);
    xTimerStart(xTimerDHT,0);

    /* A partir deste momento, o scheduler de tarefas entra em ação e as tarefas executam */
}

//função pra reconectar ao servido MQTT
void reconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {

     if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
      //subscreve no tópico
      client.subscribe("esp32/output", 1); // subscreve no tópico, nivel de qualidade: QoS 1 
      client.subscribe("esp32/lampada", 1); // subscreve no tópico, nivel de qualidade: QoS 1 
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

//Função callback chamada quando uma mensagem for recebida para subscrições:
void callback(char* topic, byte* payload, unsigned int length) {

  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);

  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensage:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");

  //aciona saída conforme msg recebida
  if (String(topic) == "esp32/output") {
    if (strMSG == "1") {        //se msg "1"
      digitalWrite(OUTPUT_2, HIGH);  //coloca saída em HIGH para ligar a Lampada 
    } else if (strMSG == "0") {  //se msg "0"
      digitalWrite(OUTPUT_2, LOW);   //coloca saída em HIGH para desligar a Lampada 
    }
  }
    if (String(topic) == "esp32/lampada") {
    if (strMSG == "1") {        //se msg "1"
      digitalWrite(OUTPUT_1, HIGH);  //coloca saída em HIGH para ligar a Lampada 
    } else if (strMSG == "0") {  //se msg "0"
      digitalWrite(OUTPUT_1, LOW);   //coloca saída em HIGH para desligar a Lampada 
    }
  }
  mqttSendJsonIO();  // publica o status dos pinos via mqtt no broker
}



/* impementação da TaskADC */
void vTaskSensor(void *pvParameters )
{
  (void) pvParameters;
  
  int adcValue;
  while(1)
  {
    adcValue = sensorRead();
    xQueueOverwrite(xFila, &adcValue);/* envia valor atual de count para fila*/
    vTaskDelay(pdMS_TO_TICKS(500)); /* Aguarda 500 ms antes de uma nova iteração*/
  }
}

/* Implemntação da vTaskDisplay  */
void vTaskPrint(void *pvParameters )
{
  (void) pvParameters;  /* Apenas para o Compilador não retornar warnings */
  int valor_recebido = 0;
  while(1)
  {   
       /* Espera até algo ser recebido na queue. 
           A leitura é obrigatória e não importa o tempo necessário 
           para acontecer (portMAX_DELAY) */
      if(xQueueReceive(xFila, &valor_recebido, portMAX_DELAY) == pdTRUE) //verifica se há valor na fila para ser lido. Espera 1 segundo
      {
       /* Uma vez recebida a informação na queue, a escreve no display LCD */
        imprimeSensorDisplay(valor_recebido);
      }
  }
}

/*Implementação da Task MQTT */
void vTaskMQTT(void *pvParameters){
  (void) pvParameters;
  //char mensagem[30];
  //UBaseType_t uxHighWaterMark;   
  int valor_recebido = 0;

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while(1)
  {
    
      if(xQueueReceive(xFila, &valor_recebido, portMAX_DELAY) == pdTRUE) //verifica se há valor na fila para ser lido. Espera 1 segundo
      {
        if(!client.connected()){
          reconect();
        }
        mqttSendJson(valor_recebido);   
         /* Para fins de teste de ocupação de stack, printa na serial o high water mark */
       //  uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
       //  Serial.print(pcTaskGetTaskName(NULL));
       //  Serial.print(" : ");
      //  Serial.println(uxHighWaterMark);

        vTaskDelay(5000);

      }
      else
      {
        Serial.println("TIMEOUT");
      }
 

  }

}


void vTaskTeclado( void * pvParameters )
{
  int pin = (int) pvParameters;

  uint8_t debouncingTime = 0;
  bool estadoAnterior = 0;

  pinMode(OUTPUT_1,OUTPUT);      
  initTeclado(pin);

  while(1)
  {
    if(leTeclado(pin)== PRESSIONADO){
      debouncingTime++;
      if((debouncingTime >= 10) && (estadoAnterior == SOLTO) )
      {

          debouncingTime = 0;
          digitalWrite(OUTPUT_1,!digitalRead(OUTPUT_1));
          mqttSendJsonIOviatask();
          estadoAnterior = PRESSIONADO;
      }
    }
    else{
      debouncingTime = 0;
      estadoAnterior = SOLTO;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

///*****************
void initTeclado(int pin){
    pinMode(pin,INPUT_PULLUP);
}

bool leTeclado(unsigned char tecla){

    return digitalRead(tecla);
}

//***
int sensorRead(){
  int buffer[100];
   int i = 0;
    int valor  = 0;

    i++;
    if(i==10) i = 0;
    buffer[i] = analogRead(SENSOR_PIN);

    for(char x = 0;x<10;x++){
         valor +=buffer[i];
    }


    return map(valor/10,0,4095,0,100);
}

void callBackTimer(TimerHandle_t pxTimer ){
  digitalWrite(LED_HEART_BEAT,HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  digitalWrite(LED_HEART_BEAT,LOW);
  }

void callBackTimerDHT(TimerHandle_t pxTimerDHT){
  mqttDHTSendJson();
}


void imprimeSensorDisplay(int valor){
  lcd.clear();
  lcd.setCursor(0, 1);
	lcd.print(" SENSOR:");
	lcd.print(valor);
 
}


void mqttSendJson(float valor1){
    //Envia a mensagem ao broker
     /// . produzindo mensagem
    DynamicJsonDocument doc(1024);
    doc["device"] = "ESP32";
    doc["analogico"] = valor1;
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish("esp32/ADC", JSONmessageBuffer);
    Serial.print("msg json enviado: ");
    Serial.println(JSONmessageBuffer);
}


void mqttSendJsonIO(void){
    //Envia a mensagem ao broker
     /// . produzindo mensagem
    DynamicJsonDocument doc(1024);
    doc["device"] = "ESP32";
    doc["OUT1"] = digitalRead(OUTPUT_1);
    doc["OUT2"] = digitalRead(OUTPUT_2);
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish("esp32/out", JSONmessageBuffer);
    Serial.print("msg json out enviado: ");
    Serial.println(JSONmessageBuffer);
}

void mqttSendJsonIOviatask(void){
    //Envia a mensagem ao broker
     /// . produzindo mensagem
    DynamicJsonDocument doc(1024);
    doc["device"] = "ESP32";
    doc["OUT1"] = digitalRead(OUTPUT_1);
    doc["OUT2"] = digitalRead(OUTPUT_2);
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish("esp32/out", JSONmessageBuffer);
    Serial.print("msg json out enviado: ");
    Serial.println(JSONmessageBuffer);
}

//////////////

void mqttDHTSendJson(){
    //Envia a mensagem ao broker
     /// . produzindo mensagem
    DynamicJsonDocument doc(1024);
    float temp;
    float hum;
     hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp = dht.readTemperature();
  if (isnan(hum) || isnan(temp)) 
  {
    
  } 
  else
  {
    doc["device"] = "ESP32";
    doc["temperatura"] = temp;
    doc["humidade"] = hum;
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish("esp32/sensor", JSONmessageBuffer);
    Serial.print("msg dht json enviado: ");
    Serial.println(JSONmessageBuffer);
  }

    
}


