#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QUANTIDADE_TAMANHOS 5
#define TEMPO_MINIMO_SEGUNDOS 0.10
#define MAX_REPETICOES 1048576

typedef void (*FuncaoOrdenacao)(int *, size_t);

typedef struct {
    const char *nome;
    FuncaoOrdenacao ordenar;
} Algoritmo;

static void trocar(int *a, int *b) {
    int temporario = *a;
    *a = *b;
    *b = temporario;
}

static void bubble_sort(int *vetor, size_t tamanho) {
    size_t i;
    size_t j;

    for (i = 0; i + 1 < tamanho; i++) {
        int houve_troca = 0;

        for (j = 0; j + 1 < tamanho - i; j++) {
            if (vetor[j] > vetor[j + 1]) {
                trocar(&vetor[j], &vetor[j + 1]);
                houve_troca = 1;
            }
        }

        if (!houve_troca) {
            break;
        }
    }
}

static void merge(int *vetor, int *auxiliar, size_t inicio,
                  size_t meio, size_t fim) {
    size_t esquerda = inicio;
    size_t direita = meio;
    size_t destino = inicio;

    while (esquerda < meio && direita < fim) {
        if (vetor[esquerda] <= vetor[direita]) {
            auxiliar[destino++] = vetor[esquerda++];
        } else {
            auxiliar[destino++] = vetor[direita++];
        }
    }

    while (esquerda < meio) {
        auxiliar[destino++] = vetor[esquerda++];
    }

    while (direita < fim) {
        auxiliar[destino++] = vetor[direita++];
    }

    memcpy(vetor + inicio, auxiliar + inicio, (fim - inicio) * sizeof(int));
}

static void merge_sort_recursivo(int *vetor, int *auxiliar,
                                 size_t inicio, size_t fim) {
    size_t meio;

    if (fim - inicio < 2) {
        return;
    }

    meio = inicio + (fim - inicio) / 2;
    merge_sort_recursivo(vetor, auxiliar, inicio, meio);
    merge_sort_recursivo(vetor, auxiliar, meio, fim);
    merge(vetor, auxiliar, inicio, meio, fim);
}

static void merge_sort(int *vetor, size_t tamanho) {
    int *auxiliar = malloc(tamanho * sizeof(int));

    if (auxiliar == NULL) {
        fprintf(stderr, "Erro ao reservar memoria para o merge sort.\n");
        exit(EXIT_FAILURE);
    }

    merge_sort_recursivo(vetor, auxiliar, 0, tamanho);
    free(auxiliar);
}

static void quick_sort_recursivo(int *vetor, long esquerda, long direita) {
    long i = esquerda;
    long j = direita;
    int pivo = vetor[esquerda + (direita - esquerda) / 2];

    while (i <= j) {
        while (vetor[i] < pivo) {
            i++;
        }
        while (vetor[j] > pivo) {
            j--;
        }
        if (i <= j) {
            trocar(&vetor[i], &vetor[j]);
            i++;
            j--;
        }
    }

    if (esquerda < j) {
        quick_sort_recursivo(vetor, esquerda, j);
    }
    if (i < direita) {
        quick_sort_recursivo(vetor, i, direita);
    }
}

static void quick_sort(int *vetor, size_t tamanho) {
    if (tamanho > 1) {
        quick_sort_recursivo(vetor, 0, (long)tamanho - 1);
    }
}

static void preencher_vetor(int *vetor, size_t tamanho) {
    size_t i;

    for (i = 0; i < tamanho; i++) {
        unsigned int parte_alta = (unsigned int)rand() << 16;
        unsigned int parte_baixa = (unsigned int)rand();
        vetor[i] = (int)(parte_alta ^ parte_baixa);
    }
}

static int esta_ordenado(const int *vetor, size_t tamanho) {
    size_t i;

    for (i = 1; i < tamanho; i++) {
        if (vetor[i - 1] > vetor[i]) {
            return 0;
        }
    }
    return 1;
}

/*
 * Adaptacao direta da funcao de tempo fornecida no AVA.
 *
 * O bloco usa as mesmas variaveis e a mesma formula:
 * tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
 *
 * A ordenacao e repetida para evitar que vetores pequenos resultem em
 * 0,000000. A copia restaura a mesma entrada antes de cada execucao.
 */
static double medir_lote_com_funcao_do_ava(
        FuncaoOrdenacao ordenar, const int *original, int *copia,
        size_t tamanho, size_t repeticoes) {
    size_t repeticao;
    clock_t inicio, fim;
    double tempo_cpu;

    inicio = clock(); /* Marca o tempo de inicio. */

    /* Trecho de codigo medido. */
    for (repeticao = 0; repeticao < repeticoes; repeticao++) {
        memcpy(copia, original, tamanho * sizeof(int));
        ordenar(copia, tamanho);
    }

    fim = clock(); /* Marca o tempo de fim. */

    /* Mesma formula fornecida no AVA: resultado em segundos. */
    tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    return tempo_cpu;
}

static double medir_tempo_das_copias(const int *original, int *copia,
                                     size_t tamanho, size_t repeticoes) {
    size_t repeticao;
    volatile int verificador = 0;
    clock_t inicio, fim;
    double tempo_cpu;

    inicio = clock();

    for (repeticao = 0; repeticao < repeticoes; repeticao++) {
        memcpy(copia, original, tamanho * sizeof(int));
        verificador ^= copia[repeticao % tamanho];
    }

    fim = clock();
    (void)verificador;
    tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    return tempo_cpu;
}

static double medir_tempo_ms(FuncaoOrdenacao ordenar, const int *original,
                             size_t tamanho, size_t *repeticoes_usadas) {
    int *copia = malloc(tamanho * sizeof(int));
    size_t repeticoes = 1;
    double tempo_total;
    double tempo_copias;
    double tempo_ordenacao;

    if (copia == NULL) {
        fprintf(stderr, "Erro ao reservar memoria para a medicao.\n");
        exit(EXIT_FAILURE);
    }

    do {
        tempo_total = medir_lote_com_funcao_do_ava(
            ordenar, original, copia, tamanho, repeticoes);
        if (tempo_total >= TEMPO_MINIMO_SEGUNDOS ||
            repeticoes == MAX_REPETICOES) {
            break;
        }
        repeticoes *= 2;
    } while (repeticoes <= MAX_REPETICOES);

    tempo_copias = medir_tempo_das_copias(
        original, copia, tamanho, repeticoes);
    tempo_ordenacao = tempo_total - tempo_copias;
    if (tempo_ordenacao < 0.0) {
        tempo_ordenacao = 0.0;
    }

    memcpy(copia, original, tamanho * sizeof(int));
    ordenar(copia, tamanho);
    if (!esta_ordenado(copia, tamanho)) {
        fprintf(stderr, "Erro: um algoritmo nao ordenou o vetor.\n");
        free(copia);
        exit(EXIT_FAILURE);
    }

    free(copia);
    *repeticoes_usadas = repeticoes;
    /*
     * Alteracao solicitada nos itens 2 e 5:
     * segundos * 1000 = milissegundos.
     */
    return (tempo_ordenacao * 1000.0) / repeticoes;
}

int main(void) {
    const size_t tamanhos[QUANTIDADE_TAMANHOS] = {100, 300, 500, 1000, 10000};
    const Algoritmo algoritmos[] = {
        {"Bubble Sort", bubble_sort},
        {"Merge Sort", merge_sort},
        {"Quick Sort", quick_sort}
    };
    const size_t quantidade_algoritmos =
        sizeof(algoritmos) / sizeof(algoritmos[0]);
    const int vetor_exemplo[] = {42, 7, 19, 3, 88, 51, 14, 1, 73, 26};
    const size_t tamanho_exemplo =
        sizeof(vetor_exemplo) / sizeof(vetor_exemplo[0]);
    FILE *arquivo;
    size_t i;
    size_t j;

    srand(2026);
    arquivo = fopen("resultados.csv", "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Nao foi possivel criar resultados.csv.\n");
        return EXIT_FAILURE;
    }

    printf("Teste com o mesmo vetor (%lu elementos):\n",
           (unsigned long)tamanho_exemplo);
    for (j = 0; j < quantidade_algoritmos; j++) {
        size_t repeticoes;
        double tempo = medir_tempo_ms(
            algoritmos[j].ordenar, vetor_exemplo, tamanho_exemplo, &repeticoes);
        printf("  %-12s: %.6f ms (media de %lu execucoes)\n",
               algoritmos[j].nome, tempo, (unsigned long)repeticoes);
    }

    fprintf(arquivo, "tamanho,bubble_ms,merge_ms,quick_ms\n");
    printf("\nVetores randomizados:\n");
    printf("%-10s %-15s %-15s %-15s\n",
           "Tamanho", "Bubble (ms)", "Merge (ms)", "Quick (ms)");

    for (i = 0; i < QUANTIDADE_TAMANHOS; i++) {
        int *original = malloc(tamanhos[i] * sizeof(int));
        double tempos[3];

        if (original == NULL) {
            fprintf(stderr, "Erro ao reservar o vetor de %lu elementos.\n",
                    (unsigned long)tamanhos[i]);
            fclose(arquivo);
            return EXIT_FAILURE;
        }

        preencher_vetor(original, tamanhos[i]);
        for (j = 0; j < quantidade_algoritmos; j++) {
            size_t repeticoes;
            tempos[j] = medir_tempo_ms(
                algoritmos[j].ordenar, original, tamanhos[i], &repeticoes);
        }

        printf("%-10lu %-15.6f %-15.6f %-15.6f\n",
               (unsigned long)tamanhos[i], tempos[0], tempos[1], tempos[2]);
        fprintf(arquivo, "%lu,%.9f,%.9f,%.9f\n",
                (unsigned long)tamanhos[i], tempos[0], tempos[1], tempos[2]);
        free(original);
    }

    fclose(arquivo);
    printf("\nResultados salvos em resultados.csv.\n");
    return EXIT_SUCCESS;
}
