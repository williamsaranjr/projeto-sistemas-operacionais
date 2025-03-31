# Projeto 📋
O projeto faz parte da avaliação da matéria de **Sistemas Operacionais**, e gira em torno do uso de **threads**, **sincronização** e **paralelismo** na linguagem C.

# Central de Emergência 🚨
 Escolhemos como tema de nosso projeto a simulação de uma **Central de Emergência**, onde teremos dois atores principais:
 - **🎧 Central:** A central será uma thread única que executará a cada 3 segundos uma busca por novas **ocorrências**. A probabilidade de que essa busca resulte em uma nova **ocorrência**
varia de **60-100%** ao decorrer da execução do programa. Sempre que houver uma **ocorrência**, a central designa uma **viatura** disponível para atende-la.
- **🚔 Viaturas:** Cada viatura é individualmente representada por uma thread e é considerada disponível se não estiver designada a uma **ocorrência**.
- **📟 Ocorrência:** É a ação que uma **viatura** realiza por um tempo indeterminado. A duração de uma ocorrência é determinada pelo **tempo de deslocamento + tempo de atendimento**, que por sua vez, são valores definidos aleatoriamente durante a ocorrência.
Esses valores também são incrementados durante a execução do programa para aumentar a dificuldade.

## Como executar
> [!IMPORTANT]
> O programa foi implementado usando a biblioteca padrão `unistd.h` que fornece uma API para o sistema **Unix**, portanto, o tempo das ocorrências pode não ser gerado corretamente ao executar o código no **Windows**, resultando na falta de aleatoriedade
no tempo das ocorrências.

Por fazer uso da biblioteca `pthread.h` da linguagem C, a compilação precisa acompanhar um flag modifier que permite o uso de multi-threading. Para compilar o programa, basta executar o comando correspondente ao seu sistema operacional:

|Sistema Operacional|Comando|
|-:|:-|
|Windows|`gcc -o central.exe central.c -mthreads`|
|Linux|`gcc -o central.elf central.c -pthread`|
