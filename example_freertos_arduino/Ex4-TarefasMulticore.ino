/*Objetivo: Descobrir o núcleo que está sendo executado em uma tarefa na configuração(setup) e no laço infinito(loop)  */

void setup() 
{ 
  Serial.begin(112500);
  delay(1000);

  /*Captura o núcleo que está executando o setup. */
  Serial.print("Setup: Executando no Núcleo: ");
  Serial.println(xPortGetCoreID());
 
  xTaskCreate
  (
    Tarefa,             /* Tarefa ou Função  */
    "Tarefa",           /* Nome da Tarefa ou Função  */
    10000,              /* Tamanho da Pilha  */
    NULL,               /* Parâmetro de Entrada  */
    2,                  /* Prioridade da Tarefa  */
    NULL                /* Identificador da Tarefa  */
  );             
  delay(2000); 
}
 
void loop() 
{
  /*Captura o núcleo que está executando o loop. */
  Serial.print("Loop Infinito: Executando no Núcleo: ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}
 
void Tarefa( void * parametro )
{
  /*Captura o núcleo que está executando a função da tarefa. */
  Serial.print("Tarefa Criada: Executando no Núcleo: ");
  Serial.println(xPortGetCoreID());
  
  /*Deleta a tarefa*/
  vTaskDelete(NULL);
}

