


//função da tarefa 1: escrever "função 1" no terminal 10x com delay 1 seg
void TaskLed1(void *pvParameters)
{
  (void) pvParameters;
  //pinMode (PIN_LED1, OUTUPUT);
  while(1)
  {
    //digitalWrite(PIN_LED1, !digitalRead(PIN_LED1));
    for(int i = 0; i<10; i++)
    {
      Serial.print("Funcao 1: ");
      Serial.println(i);
      vTaskDelay(1000);
    }
    Serial.println("Funcao 1 terminada");
  }
  vTaskDelete(NULL); //quando terminar esta função a tarefa será deletada
}

//função da tarefa 2: escrever "função 2" no terminal 10x com delay 1 seg
void TarefaDois(void*parametro) //não passa parâmetro nenhum (4º item da XTaskCreate())
{
  for(int i = 0; i<10; i++)
  {
    Serial.print("Funcao 2: ");
    Serial.println(i);
    delay(1000);
  }
    Serial.println("Funcao 2 terminada");
    vTaskDelete(NULL); //quando terminar esta função a tarefa será deletada
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  //criando task 1
  xTaskCreate(TaskLed1,"TarefaUm",10000,NULL,1,NULL);
  
  //criar tarefa 2
  xTaskCreate
  (
    TarefaDois,   //tarefa
    "TarefaDois", //nome da tarefa
    10000,        //tamanho da pilha, bytes que ocupa a tarefa
    NULL,         //parâmetro de entrada não passa nada
    2,            //prioridade 2
    NULL          //identificador da tarefa
 //   0,          // Núcleo que deseja rodar a tarefa (0 or 1)
  );
}

void loop() {
   

}
