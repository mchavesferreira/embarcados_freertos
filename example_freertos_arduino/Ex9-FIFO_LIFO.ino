/*Objetivo: Criar uma fila que passa argumentos do fila_LIFO pro fila_FIFO e outra que faça do fila_FIFO para o fila_LIFO. */

/*Cria-se dois identificadores de filas, um para fila_LIFO e outro pra fila_FIFO de fila. */
QueueHandle_t fila_FIFO;
QueueHandle_t fila_LIFO;
 
void setup() 
{ 
  Serial.begin(115200); 
  
  /* Cria duas filas com 10 elementoos e tamanho int */
  fila_FIFO = xQueueCreate( 10, sizeof( int ) );
  fila_LIFO = xQueueCreate( 10, sizeof( int ) ); 

  /*Caso fila_LIFO ou fila_FIFO estejam vazios, retorne mensagem de erro. */
  if(fila_FIFO == NULL || fila_LIFO ==NULL)
  {
    Serial.println("Erro ao criar uma das filas");
  }
}
 
void loop() 
{ 
  /* Se fila_FIFO ou fila_LIFO forem igual a vazio, retorna. */
  if(fila_FIFO == NULL || fila_LIFO == NULL )return; 

  /*Passa os argumentos de 0 a 9 para o fila_LIFO e fila_FIFO*/
  for(int i = 0; i<10; i++)
  {
    
    xQueueSendToBack(fila_FIFO, &i, 0);
    xQueueSendToFront(fila_LIFO, &i, 0);
  } 
  int elemento;
   
   /*FIFO: O primeiro a entrar na fila, será o primeiro a sair*/
  Serial.println("Fila FIFO:");
  for(int i = 0; i<10; i++)
  {
    xQueueReceive(fila_FIFO, &elemento, 0);
    Serial.print(elemento);
    Serial.print("|");
  }
  Serial.println();
  
  /*LIFO: O ultimo a entrar na fila, será o ultimo a sair.*/
  Serial.println("Fila LIFO:"); 
  for(int i = 0; i<10; i++)
  {
    xQueueReceive(fila_LIFO, &elemento, 0);
    Serial.print(elemento);
    Serial.print("|");
  }
  Serial.println();
  Serial.println("--------------");
  delay(1000);
}

