/****************************************************
 * Exempolo como criar tarefa no FreeRTOS
 * 
 * **************************************************/
/*  Bibliotecas  */

#include <Arduino.h>

/***   bibliotecas FreeRTOS  */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* definicoes de pinos */
#define LED 2

/* variaveis para armazenamento do handle das task */
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void setup() {

 Serial.begin(115200);
  // put your setup code here, to run once:
/// https://www.youtube.com/watch?v=YIZr2o0OvyU
//  xTaskCreatePinnedToCore(nome da função, "tarefa", tamanho da pilha, Parametros, prioridade, alteração manual, núcleo);

 xTaskCreate(vTask1,"TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &task1Handle);  // 
 xTaskCreate(vTask2,"TASK1", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task2Handle);


}

void loop() {


vTaskDelay(3000);


  // put your main code here, to run repeatedly:
}


void vTask1(void *pvParameters)
{
 pinMode(2, OUTPUT);
 while(1)
   {
     digitalWrite(2,!digitalRead(LED));
     vTaskDelay(pdMS_TO_TICKS(200));  /* converte o tempo milisegundos em tempo ticks  */
   }

}

void vTask2(void *pvParameters)
{
 int cont=0;
 while(1)
   {
     Serial.println("task 2: " + String(cont++));
       if (cont>=10)
         {
           if(task1Handle != NULL)
           {  
              Serial.println(" Deletando task1 ");
             vTaskDelete(task1Handle);
             digitalWrite(LED, LOW);
             task1Handle = NULL;
           }
           if(cont>15)
           {  
                Serial.println(" Deletando task2 ");
             vTaskDelete(NULL);
           }

         }
         vTaskDelay(pdMS_TO_TICKS(1000));
   }

}