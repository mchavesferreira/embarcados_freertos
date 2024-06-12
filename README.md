
#FreeRTOS 

# Conceito de RTOS

Um Sistema Operacional em Tempo Real (RTOS - Real-Time Operating System) é um tipo de sistema operacional projetado para gerenciar o tempo de execução de tarefas de forma precisa e previsível. Ao contrário dos sistemas operacionais tradicionais, que priorizam a eficiência geral e a capacidade de resposta, um RTOS garante que as tarefas sejam concluídas dentro de um tempo definido, conhecido como prazo ou deadline. Isso é crucial para aplicações onde a pontualidade é vital, como em sistemas embarcados, automação industrial, e dispositivos médicos [1].

# FreeRTOS

FreeRTOS é um dos RTOS mais populares e amplamente utilizados no mundo dos sistemas embarcados. Desenvolvido pela Real Time Engineers Ltd., FreeRTOS é um RTOS de código aberto, leve e de alto desempenho. Ele oferece uma variedade de funcionalidades que facilitam o desenvolvimento de aplicações em tempo real, como a gestão de tarefas, filas para comunicação entre tarefas, semáforos e mutexes para sincronização, e temporizadores [2]. FreeRTOS é conhecido por sua simplicidade, pequena pegada de memória e flexibilidade, o que o torna adequado para uma ampla gama de microcontroladores e aplicações.

# FreeRTOS no ESP32

O ESP32, um microcontrolador de baixo custo e alta performance da Espressif Systems, integra-se perfeitamente com o FreeRTOS, fornecendo um ambiente robusto para o desenvolvimento de aplicações em tempo real. Com seus dois núcleos de CPU, conectividade Wi-Fi e Bluetooth, e diversas interfaces periféricas, o ESP32 se beneficia das capacidades do FreeRTOS para gerenciar eficientemente as tarefas em execução [3]. Utilizando FreeRTOS no ESP32, os desenvolvedores podem criar aplicações complexas que exigem multitarefa, comunicação entre tarefas e gerenciamento preciso de recursos, garantindo um desempenho estável e previsível.

# Vantagens do FreeRTOS no Desenvolvimento

Utilizar FreeRTOS no desenvolvimento de sistemas embarcados oferece diversas vantagens. Primeiramente, a abstração e a organização das tarefas tornam o código mais modular e de fácil manutenção. Além disso, a gestão eficiente de recursos e a capacidade de priorizar tarefas críticas melhoram o desempenho e a confiabilidade do sistema. O suporte extenso e a ampla adoção de FreeRTOS na comunidade de desenvolvimento também significam que há uma vasta quantidade de recursos, tutoriais e exemplos disponíveis, facilitando a aprendizagem e a resolução de problemas [4]. Por fim, a integração com ferramentas de desenvolvimento modernas e a compatibilidade com diversos microcontroladores tornam o FreeRTOS uma escolha versátil e poderosa para projetos de sistemas embarcados.

## Referências

[1] https://www.freertos.org/RTOS.html
[2] https://www.freertos.org/about-RTOS.html
[3] https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html
[4] https://www.freertos.org/FreeRTOS_Support.html


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





