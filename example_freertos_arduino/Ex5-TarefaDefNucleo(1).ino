/*Objetivo: Criar uma tarefa e alternar os núcleos que serão usados analisando como será o comportamento do mesmo. */

/*Define a variável Nucleo, e coloca-se o valor correspondente ao núcleo que se quer usar na execução das tarefas. */
static int Nucleo = 1;


 
void setup() 
{ 
  Serial.begin(112500);
  //delay(1000);
  /* Mostra em qual Nucleo está rodando o setup.  */
  Serial.print("Iniciando configuracao no nucleo ");
  Serial.println(Nucleo);
 
  xTaskCreatePinnedToCore
  (
    Tarefa,           /* Tarefa ou Função  */
    "Tarefa",         /* Nome da Tarefa ou Função  */
    10000,            /* Tamanho da Pilha  */
    NULL,             /* Parâmetro de Entrada  */
    1,                /* Prioridade da   */
    NULL,             /* Identificador da   */
    Nucleo            /* Variável que define o núcleo que será usado na */
  ); 
  Serial.println("Tarefa Criada...");
}
 
void loop() 
{ 
  /*Plota na tela o nucleo usado no laço*/
  Serial.print("Iniciando o laco infinito no nucleo: ");
  Serial.println(xPortGetCoreID());
  //delay(1000);
  /*Cria uma condição infinita. */
  //while (true){}
}

/*Cria a Função da Tarefa que vai mostrar qual o núcleo está rodando a tarefa. */
void Tarefa( void * Parametro )
{   
  String Mensagem = " Rodando no nucleo: ";
  Mensagem = Mensagem + xPortGetCoreID();
 
  while(true)
  {
    Serial.println(Mensagem);
    //delay(1000);
  } 
}
