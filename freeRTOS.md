# Microkernel Exemplo: FreeRTOS

FreeRTOS (www.freertos.org) foi originalmente desenvolvido por Richard Barry em torno de 2003. Mais tarde o desenvolvimento e manutenção continuaram através da empresa Real Time Engineers Ltd. Em 2017 a empresa Real Time Engineers Ltd. passou o controle do projeto FreeRTOS para a Amazon Web Services (AWS – aws.amazon.com). Existem portes do FreeRTOS para mais de 30 arquiteturas de processadores diferentes. Por tratar-se de software livre existe um grande número de adaptações e variantes não oficiais. Este texto está baseado no livro “Mastering the FreeRTOS™ Real Time Kernel” e no “The FreeRTOS™ Reference Manual”. Ambos podem ser encontrados para download no site do sistema. Informações mais específicas e atualizadas também podem ser encontradas em www.freertos.org.

FreeRTOS foi criado para o mercado de aplicações embutidas ou embarcadas (embedded) de tempo real de pequeno porte as quais podem incluir tarefas com diferentes níveis de criticalidade. FreeRTOS é um microkernel que permite a execução de múltiplas threads (chamadas de tarefas na terminologia do FreeRTOS) incluindo escalonamento baseado em prioridades preemptivas e diversos mecanismos de sincronização entre threads.

Como tipicamente acontece com microkernel é relativamente pequeno em tamanho tendo em sua configuração mínima cerca de 9000 linhas de código. O código do FreeRTOS pode ser dividido em três subsistemas: gerência do processador, comunicação e sincronização entre threads e interfaceamento com o hardware. A maior parte do código do FreeRTOS é independente do hardware. A figura 16.4 ilustra sua organização em camadas.

FreeRTOS foi projetado para ser configurável o que é necessário tendo em vista o seu mercado alvo composto por uma grande gama de microcontroladores distintos e tamanhos de aplicações. Arquivos de configuração são usados para incluir no microkernel apenas as funcionalidades requeridas pelo projeto em questão. A lista atualizada de arquiteturas de computador e compiladores suportados pode ser obtida no site do sistema. Sua licença de software livre permite que o FreeRTOS possa ser usado em aplicações comerciais sem o pagamento de royalties.

No FreeRTOS é possível alocar memória estaticamente para todos os objetos gerenciados pelo microkernel (threads, queues, semáforos, grupos de eventos, etc). Desta forma em tempo de execução não existe alocação dinâmica de memória o que evita as variâncias naturalmente introduzidas por este mecanismo.

Em geral não existe proteção de memória no FreeRTOS, embora alguns poucos portes do microkernel para arquiteturas com MPU (Memory Protection Unit) ofereçam este serviço. FreeRTOS trata alocação de memória como um subsistema separado o que permite diferentes soluções conforme o tipo de aplicação e a arquitetura de computador usada. Em alguns casos o desenvolvedor da aplicação também implementa a gerência de memória a ser usada no sistema.

Threads (tarefas na terminologia do FreeRTOS) são implementadas como funções C que normalmente executam para sempre. Existem chamadas de sistema para criar, destruir, suspender e continuar threads. Também é possível alterar a prioridade de uma thread já criada. O número máximo de prioridades é configurável.

Threads com a mesma prioridade são escalonadas com fatias de tempo (ver capítulo 5) baseadas em uma interrupção periódica chamada tick interrupt (tick period) cujo período é configurável. Uma fatia de tempo é igual a um tick period.

Além de prioridades preemptivas, o escalonador pode ser configurado para usar apenas fatias de tempo. No caso de prioridades preemptivas, threads com prioridades iguais executam com fatias de tempo. Porém é também possível desligar o tick period, o que faz threads com a mesma prioridade executarem conforme a ordem de chegada na fila de aptos.

Existe uma thread ociosa (idle thread) que é criada automaticamente na inicialização para garantir que sempre exista pelo menos uma thread para executar. É possível colocar código útil da aplicação na thread ociosa porém com várias restrições. Por exemplo, a thread ociosa jamais pode ficar bloqueada.

Threads podem ficar bloqueadas a espera de eventos. Eventos podem ser de natureza temporal, quando a thread espera pela passagem do tempo, podendo ser por um certo intervalo de tempo pré-definido ou até um instante no tempo específico. Outros eventos estão relacionados com a sincronização com outras threads ou tratadores de interrupção. FreeRTOS oferece uma variada gama de mecanismos de sincronização os quais serão abordados mais adiante nesta seção.

No que diz respeito a temporizações, FreeRTOS permite que uma thread fique bloqueada por um dado intervalo de tempo através da chamada “vTaskDelay()”. Entretanto, também é possível bloquear uma thread até um instante específico de tempo usando a chamada “vTaskDelayUntil()”. A chamada “vTaskDelayUntil()” deve ser usada para implementar tarefas de tempo real com um período fixo.

FreeRTOS também oferece temporizadores em software que podem ser programados pelas threads para disparar periodicamente (auto-reload timers) ou em um instante específico no futuro (one-shot timers). Quando o temporizador dispara, uma função C (timer callback function) previamente definida pela thread é chamada. Esta função executa do início ao fim e deve terminar normalmente sem jamais ficar bloqueada. A prioridade com a qual ela executa pode ser configurada.
##  16.6.1 Tratadores de Interrupção da Aplicação

FreeRTOS permite que a aplicação instale seus próprios tratadores de interrupção. Por exemplo, device-drivers para periféricos específicos podem ser incluídos na aplicação desta forma.

Entretanto, no contexto de um tratador de interrupções, nem todas as chamadas de sistema do microkernel podem ser utilizadas. Especialmente aquelas que resultariam no bloqueio de uma thread não podem ser chamadas. FreeRTOS resolve este problema fornecendo duas versões para algumas chamadas de sistema: uma versão para ser usada por threads e outra versão para ser usada por tratadores de interrupção.

Mesmo threads com alta prioridade somente executam quando nenhum tratador de interrupção estiver executando. Logo, tratadores de interrupção do usuário e do microkernel possuem implicitamente uma prioridade mais alta do que qualquer thread. Quais interrupções são desabilitadas durante a execução de um tratador depende da arquitetura do processador usado. Desta forma, o desenvolvedor da aplicação precisa considerar aspectos relacionados com a sincronização entre threads e tratadores de interrupções conjuntamente. O tempo de execução de um tratador de interrupção pode ser reduzido se o mesmo executar apenas o essencial e ativar uma thread para realizar a maior parte do serviço. Isto é chamado “deferred interrupt processing” na terminologia do FreeRTOS.

## 16.6.2 Mecanismos de Sincronização

FreeRTOS oferece uma variada gama de mecanismos para a comunicação e sincronização entre threads. Entre os oferecidos estão:

**Basic Critical Sections:** quando regiões de código são protegidas com as macros “taskENTER_CRITICAL()” e “taskEXIT_CRITICAL()”. Elas desabilitam interrupções impedindo a preempção da thread em execução. Podem ser aninhadas pois o microkernel mantém uma contagem do número de operações ENTER e somente retorna a habilitar interrupções quando este número volta a zero.

**Suspending (Locking)** o escalonador: quando o escalonador é impedido de atuar de forma que a thread em execução não poderá ser preemptada por outras threads, porém as interrupções permanecem habilitadas e tratadores de interrupção poderão ser acionados.
Mutex: os quais incluem o mecanismo de herança de prioridade.

**Semáforos binários:** usados para que tratadores de interrupção possam liberar threads em espera sem perder interrupções que sejam sinalizadas enquanto aquela thread executa. Ele é usado para implementar a “deferred interrupt processing” na terminologia do FreeRTOS.

**Semáforos contadores:** onde as operações clássicas P e V são chamadas de “taken” e “given” respectivamente.

**Task notifications:** que permite uma thread ou um tratador de interrupção sinalizar diretamente outras threads sem a necessidade de um objeto adicional tal como um mutex ou semáforo. Trata-se de um mecanismo de baixo custo em processador e memória. Entretanto, é necessário especificar diretamente a thread que será sinalizada e somente um sinal pode estar pendente para cada thread a cada momento.

**Queues:** que são um dos principais mecanismos de comunicação providos pelo FreeRTOS. Elas permitem comunicação thread para thread, thread para tratador de interrupção e tratador de interrupção para thread. Uma Queue pode conter um número finito de dados de tamanho fixo. Tanto o número como o tamanho de cada dado são definidos na criação da Queue. Dados são normalmente removidos na mesma ordem na qual foram depositados. Os dados são efetivamente copiados para a memória da Queue e depois para a memória da thread. O acesso à Queue é sincronizado pelo microkernel. No caso da retirada de dados de uma Queue vazia, a thread pode optar por ficar bloqueada até algum dado estar disponível ou por um tempo máximo. O mesmo acontece quando a thread tenta depositar um dado em uma Queue lotada.

FreeRTOS oferece a possibilidade de uma thread permanecer bloqueada aguardando pela ocorrência da combinação de um ou mais eventos. Para isto é usado o mecanismo de “Event Groups” o qual é útil para sincronizar múltiplas threads as quais esperam por um evento qualquer entre múltiplos eventos ou pela ocorrência combinada de vários eventos.
