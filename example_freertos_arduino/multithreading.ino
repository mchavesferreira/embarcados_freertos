/* Exemplo básico de Multithreading no Arduino
   Este código de exemplo está em domínio público (ou licenciado CC0, à sua escolha).
   A menos que exigido pela legislação aplicável ou acordado por escrito,
   este software é distribuído "COMO ESTÁ" ("AS IS"), SEM GARANTIAS OU
   CONDIÇÕES DE QUALQUER TIPO, expressas ou implícitas.
*/

// Por favor leia o arquivo README.md na pasta que contém este exemplo.

#define MAX_LINE_LENGTH (64)

// Define duas tarefas para leitura e escrita na porta serial
void TaskWriteToSerial(void *pvParameters);
void TaskReadFromSerial(void *pvParameters);

// Define o handle da fila (Queue)
QueueHandle_t QueueHandle;

// Quantidade máxima de elementos na fila
const int QueueElementSize = 10;

// Estrutura de dados que será enviada pela fila
typedef struct{
  char line[MAX_LINE_LENGTH];
  uint8_t line_length;
} message_t;

// A função setup executa uma vez quando você pressiona reset
// ou liga a placa
void setup() {

  // Inicializa a comunicação serial a 115200 bits por segundo
  Serial.begin(115200);

  // Aguarda a serial iniciar
  while(!Serial){
    delay(10);
  }

  // Mensagem inicial
  Serial.println("Inicializando para executar as tarefas...");

  // Cria a fila que terá <QueueElementSize> elementos
  // cada um com tamanho `message_t`
  // e guarda o endereço da fila em <QueueHandle>
  QueueHandle = xQueueCreate(QueueElementSize, sizeof(message_t));

  // Verifica se a fila foi criada corretamente
  if(QueueHandle == NULL){
    Serial.println("A fila não pôde ser criada. Parando execução.");
    while(1) delay(1000); // trava aqui pois não é possível continuar
  }

  // Configura duas tarefas para rodarem independentemente
  xTaskCreate(
    TaskWriteToSerial,        // Função da tarefa
    "Task Write To Serial",   // Nome da tarefa (apenas informativo)
    2048,                     // Tamanho da stack
    NULL,                     // Nenhum parâmetro
    2,                        // Prioridade
    NULL                      // Handle da tarefa não utilizado
  );

  xTaskCreate(
    TaskReadFromSerial,       // Função da tarefa
    "Task Read From Serial",  // Nome da tarefa
    2048,                     // Tamanho da stack
    NULL,                     // Nenhum parâmetro
    1,                        // Prioridade
    NULL                      // Handle não utilizado
  );

  // O scheduler do FreeRTOS inicia automaticamente
  Serial.printf(
    "\nTudo que você escrever será retornado como eco.\n"
    "Comprimento máximo da linha: %d caracteres (+ terminador '\\0').\n"
    "Qualquer coisa maior será enviada como uma nova linha.\n\n",
    MAX_LINE_LENGTH - 1
  );
}

void loop(){

  // O loop fica livre para executar qualquer outra tarefa

  delay(1000); // Enquanto não estiver sendo usado, libera a CPU para outras tarefas
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

// Tarefa responsável por enviar para a serial
void TaskWriteToSerial(void *pvParameters){

  message_t message;

  for (;;){ // Uma tarefa nunca deve retornar ou terminar

    // Verificação para garantir que a fila existe
    if(QueueHandle != NULL){

      // Espera indefinidamente até receber um item da fila
      int ret = xQueueReceive(QueueHandle, &message, portMAX_DELAY);

      if(ret == pdPASS){

        // A mensagem foi recebida com sucesso
        // Envia de volta para a Serial (eco)
        Serial.printf(
          "Echo linha de tamanho %d: \"%s\"\n",
          message.line_length,
          message.line
        );

      }else if(ret == pdFALSE){

        Serial.println("A TaskWriteToSerial não conseguiu receber dados da fila");

      }
    }
  }
}


// Tarefa responsável por ler dados da serial
void TaskReadFromSerial(void *pvParameters){

  message_t message;

  for (;;){

    // Verifica se existem dados no buffer da Serial
    message.line_length = Serial.available();

    if(message.line_length > 0){

      // Verifica se a fila existe e se ainda há espaço disponível
      if(QueueHandle != NULL && uxQueueSpacesAvailable(QueueHandle) > 0){

        int max_length =
          message.line_length < MAX_LINE_LENGTH ?
          message.line_length :
          MAX_LINE_LENGTH - 1;

        // Lê os caracteres da Serial
        for(int i = 0; i < max_length; ++i){
          message.line[i] = Serial.read();
        }

        message.line_length = max_length;

        // Adiciona o caractere nulo de terminação
        message.line[message.line_length] = 0;

        // Envia a mensagem para a fila
        // O último parâmetro define o tempo de espera caso a fila esteja cheia
        int ret = xQueueSend(QueueHandle, (void*) &message, 0);

        if(ret == pdTRUE){
          // Mensagem enviada com sucesso

        }else if(ret == errQUEUE_FULL){

          // Isso não deveria ocorrer pois verificamos o espaço antes,
          // mas pode acontecer se outra tarefa enviar para a fila
          Serial.println("A TaskReadFromSerial não conseguiu enviar dados para a fila");

        }
      }

    }else{

      // Se não há dados para ler,
      // libera a CPU para outras tarefas
      delay(100);
    }
  }
}
