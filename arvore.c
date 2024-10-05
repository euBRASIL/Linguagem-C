#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define MAX_CAMINHO 1024 // Renomeando para evitar conflito com MAX_PATH

// Função para contar todos os arquivos e diretórios em um diretório e seus subdiretórios
int contar_arquivos(const char *diretorio) {
    DIR *dir;
    struct dirent *entrada;
    struct stat statbuf;
    int total = 0;
    char caminho[MAX_CAMINHO];

    if ((dir = opendir(diretorio)) == NULL) {
        perror("Error: abertura de Diretorio");
        return 0;
    }

    while ((entrada = readdir(dir)) != NULL) {
        // Ignorar "." e ".."
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        snprintf(caminho, sizeof(caminho), "%s/%s", diretorio, entrada->d_name);
        stat(caminho, &statbuf);

        if (S_ISDIR(statbuf.st_mode)) {
            // Se for diretório, contar recursivamente
            total += contar_arquivos(caminho);
        } else {
            total++;
        }
    }

    closedir(dir);
    return total;
}

// Função para gerar a árvore de arquivos e gravar em um arquivo .txt
void gerar_arvore(const char *diretorio, FILE *saida, int *contagem_atual, int total_arquivos) {
    DIR *dir;
    struct dirent *entrada;
    struct stat statbuf;
    char caminho[MAX_CAMINHO];

    if ((dir = opendir(diretorio)) == NULL) {
        perror("Erro ao abrir o diretório");
        return;
    }

    while ((entrada = readdir(dir)) != NULL) {
        // Ignorar "." e ".."
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        snprintf(caminho, sizeof(caminho), "%s/%s", diretorio, entrada->d_name);
        stat(caminho, &statbuf);

        if (S_ISDIR(statbuf.st_mode)) {
            // Se for diretório, escrever no arquivo e contar recursivamente
            fprintf(saida, "Diretório: %s\n", caminho);
            gerar_arvore(caminho, saida, contagem_atual, total_arquivos); // Corrigido de "saindo" para "saida"
        } else {
            // Se for arquivo, escrever no arquivo
            fprintf(saida, "Arquivo: %s\n", caminho);
            (*contagem_atual)++;
            
            // Mostrar barra de progresso
            float progresso = (float)(*contagem_atual) / total_arquivos * 100;
            int barra_completa = (int)(progresso / 2); // 50 blocos de progresso
            printf("\r[%-50s] %d%%", 
                   "00000000000000000000000000000000000000000000000000" + (50 - barra_completa),
                   (int)progresso);
            fflush(stdout);
        }
    }

    closedir(dir);
}

int main() {
    const char *diretorio = "."; // Diretório raiz para iniciar
    const char *arquivo_saida = "arvore.txt";
    FILE *saida;

    // Abrir o arquivo para salvar a árvore
    if ((saida = fopen(arquivo_saida, "w")) == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        return 1;
    }

    // Contar o número total de arquivos para o progresso
    int total_arquivos = contar_arquivos(diretorio);
    int contagem_atual = 0;

    printf("Total(arquivo/diretorio): %d\n", total_arquivos);
    
    // Gerar a árvore de diretórios e exibir barra de progresso
    gerar_arvore(diretorio, saida, &contagem_atual, total_arquivos);

    printf("\nProcesso ***concluido*** : %s\n", arquivo_saida);
    fclose(saida);

    return 0;
}
