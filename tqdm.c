#include <stdio.h>
#include <time.h>
#include <windows.h>  // Para usar a função Sleep()

void display_progress_bar(int progress, int total, clock_t start_time) {
    int bar_width = 50;
    int pos = (progress * bar_width) / total;

    // Calcula o tempo decorrido
    clock_t current_time = clock();
    double elapsed_seconds = (double)(current_time - start_time) / CLOCKS_PER_SEC;

    // Estima o tempo restante
    double rate = (progress > 0) ? (elapsed_seconds / progress) : 0;
    double estimated_total_time = rate * total;
    double estimated_remaining_time = estimated_total_time - elapsed_seconds;

    printf("[");
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos)
            printf("=");
        else if (i == pos)
            printf(">");
        else
            printf(" ");
    }

    // Exibe progresso, tempo estimado e iterações por segundo
    printf("] %d%% | %d/%d | %.2fs elapsed, %.2fs remaining | %.2f item/s\r",
           (progress * 100) / total, progress, total, elapsed_seconds,
           estimated_remaining_time, (progress > 0) ? (progress / elapsed_seconds) : 0.0);

    fflush(stdout);
}

int main() {
    int total = 100;
    clock_t start_time = clock();  // Marca o tempo de início

    for (int i = 0; i <= total; ++i) {
        display_progress_bar(i, total, start_time);
        Sleep(100);  // Pausa de 100 milissegundos
    }

    printf("\n");
    return 0;
}
