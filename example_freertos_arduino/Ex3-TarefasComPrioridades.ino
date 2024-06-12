/*Objetivo: Criar uma tarefa com prioridade 12. 
 * Ao executar a função, envia o nível de prioridade da mesma.
   Quando é executado o loop infinito, retorna a prioridade do 
   mesmo para efeito de comparação. */

/*Inicia o identificador de tarefa. */
TaskHandle_t tarefa;

void setup() 
{ 
  Serial.begin(115200);
  delay(1000);
 
  Serial.println("Prioridades");  

  /*Inicia a tarefa. */
  xTaskCreate
  (
    prioridade,      /* Tarefa ou Função  */
    "prioridade",    /* Nome da Tarefa ou Função  */
    10000,            /* Tamanho da Pilha  */
    NULL,             /* Parâmetro de Entrada  */
    12,               /* Prioridade da Tarefa  */
    &tarefa           /* Identificador da Tarefa é o enderedo do Identificador criado no TaskHandle_t */
  );      
}
 
void loop() 
{
  Serial.print("Loop Infinito: Prioridade = ");
  
  /*Captura a prioridade do laço infinito e imprime no terminal. Depois dá um delay de 1s*/
  Serial.println(uxTaskPriorityGet(NULL));  
  delay(1000);
}

/*Inicia a função da tarefa. */
/*A função captura a prioridade da tarefa e imprime no terminal. 
Depois de um delay de 5s diz que a tarefa está concluida e exclui a tarefa. */
void prioridade( void * parametro )
{ 
  Serial.print("Tarefa: Prioridade = ");  
  Serial.println(uxTaskPriorityGet(tarefa)); 
  delay(5000);  
  Serial.println("A Tarefa foi concluida");  
  vTaskDelete(NULL);
}

