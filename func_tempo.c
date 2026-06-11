#include <stdio.h>
#include <time.h>

int main() {
    clock_t inicio, fim;
    double tempo_cpu;
	long i;
    inicio = clock(); // Marca o tempo de in�cio

    // --- Trecho de c�digo a ser medido ---
    for( i = 0; i < 100000000; i++); 
    // -------------------------------------

    fim = clock(); // Marca o tempo de fim

    // Calcula o tempo decorrido em segundos
    tempo_cpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo de CPU: %f segundos\n", tempo_cpu);

    return 0;
}

