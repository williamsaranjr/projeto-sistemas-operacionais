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

#define MAXIMO_VIATURAS 20 // Número máximo de viaturas
#define MAXIMO_OCORRENCIAS 50 // Número máximo de ocorrências

// Variáveis que comandam o tempo da ocorrência
int MINIMO_DESLOCAMENTO = 7;
int MAXIMO_DESLOCAMENTO = 8;
int MINIMO_ATENDIMENTO = 20;
int MAXIMO_ATENDIMENTO = 15;

// Variáveis relacionadas às viaturas
pthread_t viaturas[MAXIMO_VIATURAS]; // Cada thread é uma viatura
int idsViaturas[MAXIMO_VIATURAS]; // Array de identificador único de viaturas

// Variáveis relacionadas ao controle da central
bool viaturasDisponiveis[MAXIMO_VIATURAS] = {false}; // Array para verificar se a viatura está em uso
int viaturasEmUso = 0;
int ocorrencias_atendidas = 0;
int ocorrencias_nao_atendidas = 0;
pthread_mutex_t viaturasMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para fazer alterações nas variáveis acima de forma sincronizada

// Variáveis relacionadas à dificuldade
int chance_de_crime = 80;
int viaturas_disponiveis = 4;

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

    printf("[VIATURA %04d] Chegou ao local da ocorrência em: %d minutos\n", codigo, deslocamento);
    sleep(rand() % MAXIMO_ATENDIMENTO + MINIMO_DESLOCAMENTO); // Simula o tempo de atendimento

    time_t finish = time(NULL); // Fim do atendimento
    long tempo = (long) difftime(finish, start); // Tempo do atendimento em 'minutos'
    printf("[VIATURA %04d] Atendimento concluído em: %lu minutos\n", codigo, tempo);

    pthread_mutex_lock(&viaturasMutex); // Aciona o mutex para garantir acesso exclusivo
    viaturasDisponiveis[codigo - 1] = false; // Libera a viatura
    viaturasEmUso--; // Decrementa o contador de viaturas em uso
    pthread_mutex_unlock(&viaturasMutex); // Libera o mutex

    pthread_exit(NULL); // Termina a thread
}

int main() {
    // Inicializa um timer aleatório
    srand(time(NULL));

    // Criar os IDs únicos para cada viatura
    inicializar_viaturas(idsViaturas, MAXIMO_VIATURAS);

    while (true) {
        int n = rand() % 101; // Gera um número de 0 a 100
        
        // Se o número de ocorrências atendidas e não atendidas for igual ao máximo de ocorrências, encerra o programa
        if (ocorrencias_atendidas + ocorrencias_nao_atendidas == MAXIMO_OCORRENCIAS) {
            printf("[CENTRAL] Todas as ocorrências foram atendidas...\n");
            printf("[CENTRAL] Foram necessárias %d viaturas para atender as ocorrências\n", viaturas_disponiveis);
            printf("[CENTRAL] Total de ocorrências atendidas: %d\n", ocorrencias_atendidas);
            printf("[CENTRAL] Total de ocorrências não atendidas: %d\n", ocorrencias_nao_atendidas);
            printf("[CENTRAL] Encerrando o programa...\n");
            // Cancela todas as threads de viaturas
            for (int i = 0; i < viaturas_disponiveis; i++) {
                if (viaturasDisponiveis[i]) {
                    pthread_cancel(viaturas[i]); // Cancela a thread da viatura
                }
            }
            break; // Encerra o programa após 50 ocorrências
        }

        // A cada 20 ocorrências, aumentar a dificuldade
        if ((ocorrencias_atendidas + ocorrencias_nao_atendidas) % 10 == 0 && ocorrencias_atendidas > 0) { // A cada 20 ocorrências atendidas, aumenta a dificuldade
            if (chance_de_crime < 100) {
                chance_de_crime += 10; // Aumenta a chance de crime
            }
            MINIMO_ATENDIMENTO+= 5; // Aumenta o tempo de atendimento
            MINIMO_DESLOCAMENTO+= 3; // Aumenta o tempo de deslocamento
            printf("[CENTRAL] Aumentando a dificuldade!\n");
        }

        if (n < chance_de_crime) { // Se estiver abaixo da chance de crime, uma nova ocorrência é criada
            printf("[CENTRAL] Ocorrência detectada! Buscando viaturas...\n");
            // Procura uma viatura disponível para atender a ocorrência
            for (int i = 0; i < viaturas_disponiveis; i++) {
                if (!viaturasDisponiveis[i]) { // Se a viatura não estiver em uso
                    pthread_mutex_lock(&viaturasMutex);
                    viaturasEmUso++; // Incrementa o número de viaturas em uso
                    viaturasDisponiveis[i] = true; // Marca a viatura como em uso
                    ocorrencias_atendidas++; // Incrementa o número de ocorrências atendidas
                    pthread_mutex_unlock(&viaturasMutex);

                    // Aciona a thread da viatura
                    pthread_create(&viaturas[i], NULL, atender_ocorrencia, &idsViaturas[i]);
                    printf("[CENTRAL] Viatura %04d designada para a ocorrência!\n", i + 1);
                    break;
                }
                // Se não foi possível encontrar uma viatura, incrementa o número de ocorrências não atendidas
                else if (i == viaturas_disponiveis - 1) {
                    ocorrencias_nao_atendidas++; // Incrementa o número de ocorrências não atendidas
                    printf("[CENTRAL] Todas as viaturas estão ocupadas!\n");
                }
            }
        } else {
            printf("[CENTRAL] Aguardando ocorrências...\n");
        }
        printf("[CENTRAL] Ocorrências atendidas: %d\n", ocorrencias_atendidas);
        if (ocorrencias_nao_atendidas > 0) {
            printf("[CENTRAL] Ocorrências não atendidas: %d\n", ocorrencias_nao_atendidas);
        }

        // Se o número de ocorrências não atendidas for maior que a metade do número de ocorrências atendidas
        // e o número de viaturas disponíveis for menor que o máximo, aumenta o número de viaturas disponíveis
        if (ocorrencias_nao_atendidas >= ocorrencias_atendidas / 2 && viaturas_disponiveis < MAXIMO_VIATURAS && ocorrencias_nao_atendidas > 0) {
            viaturas_disponiveis += 2;
            ocorrencias_nao_atendidas--; // Decrementa o número de ocorrências não atendidas
            printf("[CENTRAL] Aumentando o número de viaturas disponíveis! A frota agora é de: %d viaturas\n", viaturas_disponiveis);
        }

        printf("[CENTRAL] Total de viaturas em uso: %d/%d\n\n", viaturasEmUso, viaturas_disponiveis);

        sleep(2); // Espera para verificar novas ocorrências
    }

    return EXIT_SUCCESS;
}