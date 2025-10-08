# RTOS

# Conceito de RTOS

Um Sistema Operacional em Tempo Real (RTOS - Real-Time Operating System) é um tipo de sistema operacional projetado para gerenciar o tempo de execução de tarefas de forma precisa e previsível. Ao contrário dos sistemas operacionais tradicionais, que priorizam a eficiência geral e a capacidade de resposta, um RTOS garante que as tarefas sejam concluídas dentro de um tempo definido, conhecido como prazo ou deadline. Isso é crucial para aplicações onde a pontualidade é vital, como em sistemas embarcados, automação industrial, e dispositivos médicos [1].

# FreeRTOS

FreeRTOS é um dos RTOS mais populares e amplamente utilizados no mundo dos sistemas embarcados. Desenvolvido pela Real Time Engineers Ltd., FreeRTOS é um RTOS de código aberto, leve e de alto desempenho. Ele oferece uma variedade de funcionalidades que facilitam o desenvolvimento de aplicações em tempo real, como a gestão de tarefas, filas para comunicação entre tarefas, semáforos e mutexes para sincronização, e temporizadores [2]. FreeRTOS é conhecido por sua simplicidade, pequena pegada de memória e flexibilidade, o que o torna adequado para uma ampla gama de microcontroladores e aplicações.

# FreeRTOS no ESP32

O ESP32, um microcontrolador de baixo custo e alta performance da Espressif Systems, integra-se perfeitamente com o FreeRTOS, fornecendo um ambiente robusto para o desenvolvimento de aplicações em tempo real. Com seus dois núcleos de CPU, conectividade Wi-Fi e Bluetooth, e diversas interfaces periféricas, o ESP32 se beneficia das capacidades do FreeRTOS para gerenciar eficientemente as tarefas em execução [3]. Utilizando FreeRTOS no ESP32, os desenvolvedores podem criar aplicações complexas que exigem multitarefa, comunicação entre tarefas e gerenciamento preciso de recursos, garantindo um desempenho estável e previsível.

# Vantagens do FreeRTOS no Desenvolvimento

Utilizar FreeRTOS no desenvolvimento de sistemas embarcados oferece diversas vantagens. Primeiramente, a abstração e a organização das tarefas tornam o código mais modular e de fácil manutenção. Além disso, a gestão eficiente de recursos e a capacidade de priorizar tarefas críticas melhoram o desempenho e a confiabilidade do sistema. O suporte extenso e a ampla adoção de FreeRTOS na comunidade de desenvolvimento também significam que há uma vasta quantidade de recursos, tutoriais e exemplos disponíveis, facilitando a aprendizagem e a resolução de problemas [4]. Por fim, a integração com ferramentas de desenvolvimento modernas e a compatibilidade com diversos microcontroladores tornam o FreeRTOS uma escolha versátil e poderosa para projetos de sistemas embarcados.

# Programação Concorrente

**Programação Concorrente** é um paradigma de programação em que múltiplas tarefas ou processos são executados ao mesmo tempo, compartilhando recursos do sistema como memória e processador. Ao contrário da programação sequencial, onde as instruções são executadas uma após a outra, a programação concorrente permite que várias operações sejam realizadas simultaneamente, melhorando a eficiência e a capacidade de resposta de um sistema.

Em sistemas embarcados e em tempo real, a programação concorrente é especialmente importante, pois permite a execução de múltiplas operações críticas de forma independente e paralela. Por exemplo, enquanto uma tarefa está lidando com a comunicação de dados, outra pode estar processando leituras de sensores, e uma terceira pode estar controlando um atuador. Isso é fundamental para garantir que todas as partes do sistema funcionem harmoniosamente e dentro dos limites de tempo especificados.

A programação concorrente é implementada utilizando técnicas como **multi threading**, onde várias threads são criadas e gerenciadas pelo sistema operacional; **filas (queues)**, que são usadas para a comunicação ordenada entre tarefas; **mutexes**, que garantem acesso exclusivo a recursos compartilhados; e **semáforos**, que controlam o acesso simultâneo a múltiplas instâncias de um recurso.

A complexidade da programação concorrente vem da necessidade de gerenciar corretamente a sincronização entre tarefas, evitar condições de corrida, deadlocks e outros problemas que podem surgir quando múltiplas tarefas tentam acessar os mesmos recursos ao mesmo tempo. No entanto, quando bem implementada, a programação concorrente resulta em sistemas mais robustos, eficientes e capazes de atender às demandas de tempo real de aplicações críticas.

## Multi Threading

**Multi Threading** é a capacidade de um sistema operacional de gerenciar a execução simultânea de múltiplas threads (tarefas) dentro de um único processo. Em sistemas embarcados, isso permite a execução de várias tarefas de forma concorrente, melhorando a eficiência e a capacidade de resposta do sistema. Cada thread é uma unidade independente de execução, que compartilha recursos como memória e processador com outras threads. A utilização de multi threading é essencial para desenvolver aplicações complexas que precisam realizar múltiplas operações ao mesmo tempo, como leitura de sensores, comunicação de dados e controle de atuadores [5].


# 📌 Tarefas (*Tasks*) no FreeRTOS

### O que é uma *task*?

* Uma **task** é a unidade básica de execução no **FreeRTOS**, equivalente a uma *thread* em sistemas operacionais.
* Cada task é uma função em C que **roda concorrentemente** com outras, gerenciada pelo **escalonador do FreeRTOS**.
* O programador define:

  * O que a task faz (*função da task*).
  * Sua prioridade.
  * Tamanho da pilha.
  * Parâmetros iniciais.

---

## 🧩 Partes constituintes de uma task

1. **Função da task**

   * Estrutura típica em C:

     ```c
     void vTaskFunction(void *pvParameters) {
         for( ;; ) {
             // Código da tarefa
         }
     }
     ```
   * Deve ter **loop infinito** (`for(;;)`), pois não retorna.

2. **Contexto de execução**

   * Registradores, ponteiros e variáveis locais preservados pelo kernel.

3. **Stack (Pilha da tarefa)**

   * Espaço de memória privado de cada tarefa.

4. **Task Control Block (TCB)**

   * Estrutura interna usada pelo kernel para armazenar:

     * Estado (Running, Ready, Blocked, Suspended).
     * Ponteiro da pilha.
     * Prioridade.
     * Parâmetros passados.

---

## ⚙️ Principais funções da biblioteca FreeRTOS (fluxo de vida da task)

### 🔹 **Criação**

* **`xTaskCreate()`**
  Cria uma nova tarefa.

  ```c
  BaseType_t xTaskCreate(
      TaskFunction_t pvTaskCode,   // Função da tarefa
      const char * const pcName,   // Nome (debug)
      configSTACK_DEPTH_TYPE usStackDepth, // Tamanho da pilha
      void *pvParameters,          // Parâmetros
      UBaseType_t uxPriority,      // Prioridade
      TaskHandle_t *pxCreatedTask  // Handle (opcional)
  );
  ```
* **`xTaskCreateStatic()`**
  Versão sem alocação dinâmica (memória estática).

---

### 🔹 **Execução e Controle**

* **`vTaskDelay(ticks)`**
  Bloqueia a task por um período de tempo.

* **`vTaskDelayUntil()`**
  Delay relativo ao último “despertar”, útil para tarefas periódicas.

* **`vTaskSuspend(TaskHandle_t xTask)`**
  Suspende a execução da tarefa.

* **`vTaskResume(TaskHandle_t xTask)`**
  Retoma a execução de uma tarefa suspensa.

* **`vTaskPrioritySet(TaskHandle_t, UBaseType_t)`**
  Altera prioridade em tempo de execução.

---

### 🔹 **Sincronização e Comunicação**

* **Filas (Queues):** `xQueueCreate`, `xQueueSend`, `xQueueReceive`.
* **Semáforos:** `xSemaphoreCreateBinary`, `xSemaphoreTake`, `xSemaphoreGive`.
* **Mutexes:** `xSemaphoreCreateMutex`, usados para exclusão mútua.
* **Event Groups:** `xEventGroupSetBits`, `xEventGroupWaitBits`.

---

### 🔹 **Finalização**

* **`vTaskDelete(TaskHandle_t xTask)`**
  Remove a tarefa do sistema.

  * Se for chamado com `NULL`, a própria tarefa se deleta.
  * Memória da pilha e TCB é liberada (no caso de `xTaskCreate`, que usa heap).



## 📌 Estados de Tarefa no FreeRTOS

### 1. **Running (Executando)**

* Estado da tarefa que **está usando a CPU naquele instante**.
* Apenas **uma tarefa por núcleo** pode estar em Running.
* O escalonador escolhe a tarefa de maior prioridade **entre as que estão em Ready**.

---

### 2. **Ready (Pronta)**

* A tarefa **está pronta para executar**, mas ainda não foi escolhida pelo escalonador.
* Ela já tem todos os recursos necessários, mas aguarda a CPU.
* Se houver outra tarefa com prioridade maior, esta continuará aguardando em Ready.

---

### 3. **Blocked (Bloqueada)**

* A tarefa **está aguardando um evento ou temporizador**.
* Exemplos:

  * `vTaskDelay()` → espera até o tempo expirar.
  * `xQueueReceive()` → espera até a chegada de uma mensagem.
  * `xSemaphoreTake()` → espera até que o semáforo esteja disponível.
* Ao ocorrer o evento (ou timeout), o kernel move a tarefa de volta para **Ready**.
* Não consome CPU enquanto está em Blocked.

---

### 4. **Suspended (Suspensa)**

* Estado em que a tarefa foi **removida manualmente** da lista de tarefas elegíveis.
* Feito com `vTaskSuspend()`.
* Ela **não volta automaticamente**: apenas `vTaskResume()` a coloca novamente em **Ready**.
* Usado em casos específicos, por exemplo:

  * Suspender temporariamente uma tarefa de log quando não é necessária.
  * Reduzir consumo de RAM/CPU em certas fases da aplicação.

### 5. Finalizada

* Para “matar” uma tarefa, usamos:

```c
vTaskDelete(TaskHandle_t xTask);
```


1. O **Task Control Block (TCB)** da tarefa é liberado.
2. A **pilha (stack)** associada à tarefa também é liberada (quando criada com `xTaskCreate`, que usa alocação dinâmica).
3. A tarefa deixa de aparecer nas listas do escalonador.
4. Depois disso, a tarefa não existe mais no sistema → não consome CPU nem RAM.



## Queue (Filas)

<img src= https://github.com/mchavesferreira/embarcados_freertos/blob/main/imagens/filas.gif>

**Queue (Fila)** é uma estrutura de dados usada para armazenar e gerenciar um conjunto de elementos em que a ordem de inserção é importante. Em um RTOS, filas são frequentemente usadas para a comunicação entre tarefas. As filas operam segundo o princípio FIFO (First In, First Out), onde o primeiro elemento inserido é o primeiro a ser retirado. Elas são úteis para enviar mensagens ou dados entre diferentes tarefas, garantindo que a comunicação seja ordenada e eficiente [6]. Por exemplo, uma tarefa de leitura de sensor pode colocar dados em uma fila, e uma tarefa de processamento pode retirar esses dados para análise.

<img src=https://github.com/mchavesferreira/embarcados_freertos/blob/main/imagens/rtos-queue-fifo-animacao.gif>

* **Funções principais:**

  * `xQueueCreate(tamanho, tamanhoItem)` → cria a fila.
  * `xQueueSend(fila, &dado, timeout)` → envia um item para a fila.
  * `xQueueReceive(fila, &dado, timeout)` → retira um item da fila.


## Semáforo (Semaphore)

**Semaphore (Semáforo)** é outro mecanismo de sincronização usado para controlar o acesso a recursos compartilhados. Semáforos podem ser contadores binários (binários) ou contadores (contadores). Um semáforo binário funciona de forma similar a um mutex, permitindo ou bloqueando o acesso a um recurso único. Semáforos contadores, por outro lado, permitem que um número específico de tarefas acesse um recurso simultaneamente [8]. Eles são úteis para gerenciar múltiplos instâncias de recursos limitados, como conexões de rede ou slots de buffer.

* **Funções principais:**

  * `xSemaphoreCreateBinary()` → cria o semáforo binário.
  * `xSemaphoreTake(sem, timeout)` → tarefa tenta “pegar” (se não disponível, fica bloqueada).
  * `xSemaphoreGive(sem)` → libera, acordando quem espera.
    
## Mutex (Mutual Exclusion Semaphore)

 Garantir **exclusão mútua** no acesso a recursos compartilhados (ex.: porta serial, memória, periféricos) Garantir **exclusão mútua** no acesso a recursos compartilhados (ex.: porta serial, memória, periféricos)
 
**Mutex (Mutual Exclusion)** é um mecanismo de sincronização usado para evitar que múltiplas tarefas acessem um recurso compartilhado ao mesmo tempo. Quando uma tarefa precisa acessar um recurso compartilhado, ela "tranca" o mutex, impedindo outras tarefas de acessar o mesmo recurso até que o mutex seja "destrancado". Isso garante que os dados não sejam corrompidos por acessos simultâneos e que o recurso seja usado de forma segura e controlada [7]. Mutexes são essenciais em ambientes de multi threading para proteger variáveis compartilhadas, buffers e outras estruturas de dados.

* **Funções principais:**

  * `xSemaphoreCreateMutex()` → cria o mutex.
  * `xSemaphoreTake(mutex, timeout)` → trava o recurso.
  * `xSemaphoreGive(mutex)` → libera o recurso.

 
## Event Group

Event Group: conjunto de bandeiras (bits) para esperar múltiplos eventos.

Event Groups (Grupos de Eventos) são mecanismos de sincronização que usam bits para representar eventos. Cada bit indica se um evento ocorreu, e as tarefas podem definir ou aguardar esses bits. Eles permitem que uma tarefa espere um evento específico ou múltiplos eventos antes de continuar, evitando a necessidade de vários semáforos individuais.

* **Funções principais:**

  * `xEventGroupSetBits(grupo, bits)` → define quais eventos ocorreram.
  * `xEventGroupWaitBits(grupo, bits, limpar, esperarTodos, timeout)` → tarefa espera até que os bits de evento desejados estejam setados.
  * 

# Primeiro Código FreeRTOS

Exemplo 1:

<details><summary>Utilizando Expressif</summary>
<p>

```ruby

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

void vTaskCode(void * pvParameters)
{
    for(;;)
    {
        printf("Hello from Task!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(vTaskCode, "TaskName", 2048, NULL, 1, NULL);
}

```
</details>

Exemplo 2:

<details><summary>IDE-Arduino, considerando escolha do processador</summary>
<p>

```ruby


#include <Arduino.h>

void TaskBlink(void *pvParameters) {
  (void) pvParameters;

  // Configure the LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) {
    // Toggle the LED state
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // Delay for a second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Create a new task pinned to core 0
  xTaskCreatePinnedToCore(
    TaskBlink,      // Função que implementa a tarefa.
    "Blink",        // Nome textual para a tarefa.
    1024,           // Tamanho da pilha em palavras, não em bytes.
    NULL,           // Parâmetro passado para a tarefa.
    1,              // Prioridade com a qual a tarefa é criada.
    NULL,           // Usado para retornar um handle pelo qual a tarefa criada pode ser referenciada.
    0);             // Núcleo no qual a tarefa será executada (0 ou 1).
}

void loop() {
  // Empty. Things are done in Tasks.
}

```
</details>


## O que é `xTaskCreatePinnedToCore` (ESP32)

É uma **extensão do ESP-IDF** (não faz parte do FreeRTOS “puro”) que cria uma tarefa **presa a um núcleo específico** do ESP32 (dual-core).
Assinatura típica:

```c
BaseType_t xTaskCreatePinnedToCore(
    TaskFunction_t pvTaskCode,      // função da task
    const char * const pcName,      // nome (debug)
    const uint32_t usStackDepth,    // tamanho da pilha (words)
    void *pvParameters,             // parâmetro
    UBaseType_t uxPriority,         // prioridade
    TaskHandle_t *pvCreatedTask,    // (opcional) handle de saída
    const BaseType_t xCoreID        // 0 ou 1 no ESP32 clássico
);
```

* `xCoreID = 0` → fixa no **core 0** (tradicionalmente onde rodam Wi-Fi/BLE e parte do sistema).
* `xCoreID = 1` → fixa no **core 1** (onde o `loopTask` do Arduino costuma rodar).
* Em targets *single-core* (ex.: ESP32-S2), “pinar” é irrelevante (só há o core 0).
* Em IDF mais recentes existe também o conceito de **afinidade** de CPU (ex.: `tskNO_AFFINITY` e `vTaskCoreAffinitySet()`), mas `xTaskCreatePinnedToCore` continua muito usado no ecossistema ESP32/Arduino.


## Simulação

<img srr=https://github.com/mchavesferreira/embarcados_freertos/assets/63993080/8d63cd6d-9467-4a49-8d63-73d8b0c1b204>

<a href=https://wokwi.com/projects/400503259194429441>FreeRTOS - Como criar tarefas usando FreeRTOS no ESP32</a>

<a href=https://wokwi.com/projects/400503009290433537>Tasks com GPIO</a>

## FreeRTOS na prática

Acesse estes exemplos: 

https://github.com/mchavesferreira/embarcados_freertos/tree/main/exemplos_scripts_rtos

https://github.com/mchavesferreira/embarcados_freertos/tree/main/example_freertos_arduino

https://github.com/FBSeletronica/Curso-primeiros-passos-com-freeRTOS-Codigos

## Fórum de Sistemas Embarcados e IoT 2025


### FreeRTOS x Zephyr

Palestra: Zephyr RTOS: Primeiros Passos
Palestrante: Jorge Guzman
https://github.com/JorgeGzm/EmbarcadosIot2025

### 🔄 **Resumo Comparativo: FreeRTOS vs Zephyr**

| Característica               | **FreeRTOS**                                        | **Zephyr RTOS**                                                    |
| ---------------------------- | --------------------------------------------------- | ------------------------------------------------------------------ |
| **Licença**                  | MIT (muito permissiva)                              | Apache 2.0 (também permissiva)                                     |
| **Modelo de RTOS**           | Kernel mínimo, tempo real preemptivo                | Kernel modular, suportando preemptivo e cooperativo                |
| **Arquitetura**              | Monolítica e leve                                   | Modular, com subsistemas integrados (drivers, stack de rede, etc.) |
| **Alocação de Memória**      | Suporta dinâmica e estática                         | Preferência por alocação estática (mas suporta dinâmica)           |
| **API**                      | Própria e enxuta                                    | POSIX-like (inspirada em padrões), mais robusta                    |
| **Suporte a dispositivos**   | Precisa de BSP e HAL externos (Ex: STM32 HAL, etc.) | Já inclui HALs e drivers no próprio RTOS                           |
| **Rede / TCP/IP**            | Requer pilhas externas (como LwIP, FreeRTOS+TCP)    | Stack de rede integrada e configurável                             |
| **Segurança e Certificação** | FreeRTOS SAFETY e AWS Certifiable Kernel            | Foco em segurança desde o início, suporte a Trusted Firmware, etc. |
| **Gerenciamento de Energia** | Básico, depende de implementação do usuário         | Subsystem de gerenciamento de energia avançado                     |
| **Comunidade / Suporte**     | Ampla adoção na indústria, documentação direta      | Forte suporte da Linux Foundation, documentação extensiva          |
| **Ferramentas**              | Simples, usa makefiles ou CMake                     | Integração com **Zephyr SDK**, **west**, e **CMake**               |
| **Curva de Aprendizado**     | Curta, ideal para iniciantes                        | Mais íngreme, devido à complexidade e flexibilidade                |

---

### 📌 **Quando usar qual?**

| Caso de Uso                                                                              | Melhor Opção                                   |
| ---------------------------------------------------------------------------------------- | ---------------------------------------------- |
| Projetos pequenos, simples, com controle total do sistema                                | **FreeRTOS**                                   |
| Projetos maiores, conectados, com recursos como drivers integrados, segurança, BLE, etc. | **Zephyr**                                     |
| Sistemas com necessidade de certificação funcional (ex: automotivo, industrial)          | **Ambos são viáveis**, com versões específicas |

---

### 🔧 Exemplo prático

* **FreeRTOS**: ideal para um sistema bare-metal com um microcontrolador STM32 que precisa apenas de algumas tasks simples.
* **Zephyr**: ideal para um dispositivo IoT com Wi-Fi/BLE, atualizações OTA, criptografia, e suporte a múltiplas plataformas.


## Referências


[1] https://www.freertos.org/RTOS.html

[2] https://www.freertos.org/about-RTOS.html

[3] https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html

[4] https://www.freertos.org/FreeRTOS_Support.html

[5] https://www.freertos.org/implementing-a-task.html  

[6] https://www.freertos.org/Embedded-RTOS-Queues.html  

[7] https://www.freertos.org/Real-time-RTOS-mutexes.html  

[8] https://www.freertos.org/Real-time-RTOS-semaphores.html  

[9] https://en.wikipedia.org/wiki/Concurrent_computing  

[10] https://www.geeksforgeeks.org/introduction-of-parallel-computing/  

[11] https://www.cs.cmu.edu/~fp/courses/15440-s14/lectures/02-concurrency.pdf  


