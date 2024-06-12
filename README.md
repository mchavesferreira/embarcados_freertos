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

## Queue

**Queue (Fila)** é uma estrutura de dados usada para armazenar e gerenciar um conjunto de elementos em que a ordem de inserção é importante. Em um RTOS, filas são frequentemente usadas para a comunicação entre tarefas. As filas operam segundo o princípio FIFO (First In, First Out), onde o primeiro elemento inserido é o primeiro a ser retirado. Elas são úteis para enviar mensagens ou dados entre diferentes tarefas, garantindo que a comunicação seja ordenada e eficiente [6]. Por exemplo, uma tarefa de leitura de sensor pode colocar dados em uma fila, e uma tarefa de processamento pode retirar esses dados para análise.

## Mutex

**Mutex (Mutual Exclusion)** é um mecanismo de sincronização usado para evitar que múltiplas tarefas acessem um recurso compartilhado ao mesmo tempo. Quando uma tarefa precisa acessar um recurso compartilhado, ela "tranca" o mutex, impedindo outras tarefas de acessar o mesmo recurso até que o mutex seja "destrancado". Isso garante que os dados não sejam corrompidos por acessos simultâneos e que o recurso seja usado de forma segura e controlada [7]. Mutexes são essenciais em ambientes de multi threading para proteger variáveis compartilhadas, buffers e outras estruturas de dados.

## Semaphore

**Semaphore (Semáforo)** é outro mecanismo de sincronização usado para controlar o acesso a recursos compartilhados. Semáforos podem ser contadores binários (binários) ou contadores (contadores). Um semáforo binário funciona de forma similar a um mutex, permitindo ou bloqueando o acesso a um recurso único. Semáforos contadores, por outro lado, permitem que um número específico de tarefas acesse um recurso simultaneamente [8]. Eles são úteis para gerenciar múltiplos instâncias de recursos limitados, como conexões de rede ou slots de buffer.



#Primeiro Código FreeRTOS

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


## Simulação

![simulacao](https://github.com/mchavesferreira/embarcados_freertos/assets/63993080/8d63cd6d-9467-4a49-8d63-73d8b0c1b204)

<a href=https://wokwi.com/projects/400503259194429441>FreeRTOS - Como criar tarefas usando FreeRTOS no ESP32 Copy</a>

<a href=https://wokwi.com/projects/400503009290433537>Tasks com GPIO</a>


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



