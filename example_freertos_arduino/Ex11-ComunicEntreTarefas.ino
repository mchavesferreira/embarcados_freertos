/*Objetivo: Passar itens de uma tarefa pra outra. */

/*Inicia a fila*/
QueueHandle_t fila;
int tamanhodafila = 5;
 
void setup() 
{ 
  Serial.begin(115200); 
  /* Cria uma fila de até 50 elementos com tamanho int  */
  fila = xQueueCreate( tamanhodafila, sizeof( int ) ); 
  /* Se a fila estiver vazia retorna mensagem de erro */
  if(fila == NULL)
  {
    Serial.println("Erro ao criar a fila");
  } 
  
  xTaskCreate
  (
    TarefaEnviaMsg,   /* Tarefa ou Função  */
    "TarefaEnviaMsg",       /* Nome da Tarefa ou Função  */
    10000,            /* Tamanho da Pilha  */
    NULL,             /* Parâmetro de Entrada  */
    1,                /* Prioridade da Tarefa  */
    NULL              /* Identificador da Tarefa  */
  );            
 
  xTaskCreate
  (
    TarefaRecebeMsg, /* Tarefa ou Função  */
    "TarefaRecebeMsg",     /* Nome da Tarefa ou Função  */
    10000,            /* Tamanho da Pilha  */
    NULL,             /* Parâmetro de Entrada  */
    1,                /* Prioridade da Tarefa  */
    NULL              /* Identificador da Tarefa  */
  );           
 
}
 
void loop() 
{
  delay(5000);
}

void TarefaEnviaMsg( void * parameter )
{ 
  /* Insere na fila os valores de i(0 até (tamanhodafila - 1)) */
  for( int i = 0; i<tamanhodafila; i++ )
  {
    xQueueSendToBack(fila, &i, 100000);
    //xQueueSendToFront(fila, &i, portMAX_DELAY);
    delay(1000);
  } 
    
  /* Exclui a tarefa atual */
  vTaskDelete( NULL ); 
}
 
void TarefaRecebeMsg( void * parameter)
{
  int elemento; 
  
  /* Insere os elementos da fila a variavel elemento, e imprime na tela. (FIFO) */
  for( int i = 0; i < tamanhodafila; i++ )
  { 
    xQueueReceive(fila, &elemento, portMAX_DELAY);
    Serial.print(elemento);
    Serial.print("|");
    delay(1000);
  }
   
  /* Exclui a tarefa atual */ 
  vTaskDelete( NULL ); 
}

