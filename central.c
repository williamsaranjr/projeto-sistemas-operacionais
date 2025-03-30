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

// Variáveis que comandam o tempo da ocorrência
const int MINIMO_DESLOCAMENTO = 2;
const int MAXIMO_DESLOCAMENTO = 5;
const int MINIMO_ATENDIMENTO = 8;
const int MAXIMO_ATENDIMENTO = 20;

// Variáveis relacionadas às viaturas
pthread_t viaturas[NUMERO_VIATURAS]; // Cada thread é uma viatura
int idsViaturas[NUMERO_VIATURAS]; // Array de identificador único de viaturas

// Variáveis relacionadas ao controle da central
bool viaturasDisponiveis[NUMERO_VIATURAS] = {false}; // Array para verificar se a viatura está em uso
int viaturasEmUso = 0;
pthread_mutex_t viaturasMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para fazer alterações nas variáveis acima de forma sincronizada

// Variáveis relacionadas à dificuldade
int chance_de_crime = 60;

// Gerar os identificadores únicos de cada viatura
void inicializar_viaturas(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = i + 1; // Inicializa os IDs das viaturas
    }
}

void* atender_ocorrencia(void* arg) {
    time_t start = time(NULL); // Marca o início do atendimento
    int codigo = *(int*)arg; // Código identificador

    printf("[VIATURA %04d] Se deslocando ao local da ocorrência\n", codigo);
    int deslocamento = rand() % MAXIMO_DESLOCAMENTO + MINIMO_DESLOCAMENTO;
    sleep(deslocamento); // Simula o tempo de deslocamento

    printf("[VIATURA %04d] Chegou ao local da ocorrência em %d minutos\n", codigo, deslocamento);
    sleep(rand() % MAXIMO_ATENDIMENTO + MINIMO_DESLOCAMENTO); // Simula o tempo de atendimento

    time_t finish = time(NULL); // Fim do atendimento
    long tempo = (long) difftime(finish, start); // Tempo do atendimento em 'minutos'
    printf("[VIATURA %04d] Atendimento concluído em %lu minutos\n", codigo, tempo);

    pthread_mutex_lock(&viaturasMutex);
    viaturasDisponiveis[codigo - 1] = false; // Libera a viatura
    viaturasEmUso--; // Decrementa o contador de viaturas em uso
    pthread_mutex_unlock(&viaturasMutex);
    pthread_exit(NULL); // Termina a thread
}

int main() {
    // Inicializa um timer aleatório
    srand(time(NULL));

    inicializar_viaturas(idsViaturas, NUMERO_VIATURAS);

    while (true) {
        int n = rand() % 101; // Gera um número de 0 a 100

        if (n < chance_de_crime) { // Se estiver abaixo da chance de crime, uma nova ocorrência é criada
            printf("[CENTRAL] Ocorrência detectada! Buscando viaturas...\n");
            for (int i = 0; i < NUMERO_VIATURAS; i++) {
                if (!viaturasDisponiveis[i]) { // Se a viatura não estiver em uso
                    pthread_mutex_lock(&viaturasMutex);
                    viaturasEmUso++; // Incrementa o número de viaturas em uso
                    viaturasDisponiveis[i] = true; // Marca a viatura como em uso
                    pthread_mutex_unlock(&viaturasMutex);
                    pthread_create(&viaturas[i], NULL, atender_ocorrencia, &idsViaturas[i]);
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
        sleep(3); // Espera 3 minutos para verificar novas ocorrências
    }
}