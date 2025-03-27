// William Saran dos Santos Junior - 10420128
// Julia Vitoria Bomfim do Nascimento - 10425604
// Andrey Bezerra Virginio dos Santos - 10420696
// Igor Silva Araujo - 10428505

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define NUMERO_VIATURAS 15

pthread_t threads[NUMERO_VIATURAS];
bool viaturas[NUMERO_VIATURAS] = {false}; // Array para verificar se a viatura está em uso
int thread_ids[NUMERO_VIATURAS];
int viaturasEmUso = 0;

void inicializar_viaturas(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = i + 1; // Inicializa os IDs das viaturas
    }
}

void* atender_ocorrencia(void* arg) {
    time_t start = time(NULL); // Marca o início do atendimento
    int codigo = *(int*)arg; // Código identificador

    printf("[VIATURA %d] Se deslocando ao local da ocorrência\n", codigo);
    sleep(rand() % 5 + 5); // Simula o tempo de deslocamento

    printf("[VIATURA %d] Chegou ao local da ocorrência\n", codigo);
    sleep(rand() % 11 + 20); // Simula o tempo de atendimento

    time_t finish = time(NULL); // Fim do atendimento
    long tempo = (long) difftime(finish, start); // Tempo do atendimento em 'minutos'
    printf("[VIATURA %d] Atendimento concluído em %lu minutos\n", codigo, tempo);

    viaturas[codigo - 1] = false; // Libera a viatura
    viaturasEmUso--; // Decrementa o contador de viaturas em uso
    pthread_exit(NULL); // Termina a thread
}

int main() {
    srand(time(NULL));

    inicializar_viaturas(thread_ids, NUMERO_VIATURAS);

    while (true) {
        int n = rand() % 101; // Gera um número de 1 a 100

        if (n < 100) { // Se for menor que 60, há uma ocorrência
            n = rand() % NUMERO_VIATURAS; // Seleciona uma viatura aleatória
            printf("[CENTRAL] Ocorrência detectada! Buscando viaturas...\n");
            for (int i = 0; i < NUMERO_VIATURAS; i++) {
                if (!viaturas[i]) { // Se a viatura não estiver em uso
                    viaturasEmUso++; // Incrementa o número de viaturas em uso
                    viaturas[i] = true; // Marca a viatura como em uso
                    pthread_create(&threads[i], NULL, atender_ocorrencia, &thread_ids[i]);
                    printf("[CENTRAL] Viatura %d designada para a ocorrência\n", i + 1);
                    break;
                } else if (i == NUMERO_VIATURAS - 1) {
                    printf("[CENTRAL] Todas as viaturas estão ocupadas\n");
                }
            }
        } else {
            printf("[CENTRAL] Aguardando ocorrências...\n");
        }
        printf("[CENTRAL] Total de viaturas em uso: %d\n\n", viaturasEmUso);
        sleep(3); // Espera 3 minuto para verificar novas ocorrências
    }
}