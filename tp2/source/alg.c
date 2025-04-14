#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "alg.h"

/* -------------------------- Leitura de Instância -------------------------- */

City* ler_instancia(char *filename, int *n) {
    FILE *arq = fopen(filename, "r");
    if (!arq) return NULL;
    char linha[128];
    while (fgets(linha, sizeof(linha), arq)) {
        if (strncmp(linha, "DIMENSION", 9) == 0)
            sscanf(linha, "DIMENSION: %d", n);
        if (strncmp(linha, "NODE_COORD_SECTION", 18) == 0)
            break;
    }
    City *cities = malloc((*n) * sizeof(City));
    for (int i = 0; i < *n; i++) {
        if (fscanf(arq, "%d %lf %lf", &cities[i].id, &cities[i].x, &cities[i].y) != 3) {
            fprintf(stderr, "Erro ao ler dados da cidade %d\n", i);
            free(cities);
            fclose(arq);
            return NULL;
        }
    }
    fclose(arq);
    return cities;
}

/* -------------------------- Matriz de Distâncias -------------------------- */

double calc_dist(City a, City b) {
    double dx = b.x - a.x, dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

double** gerar_matriz_dist(City *cities, int n) {
    double **matriz = malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        matriz[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
            matriz[i][j] = calc_dist(cities[i], cities[j]);
    }
    return matriz;
}

void liberar_matriz(double **matriz, int n) {
    for (int i = 0; i < n; i++)
        free(matriz[i]);
    free(matriz);
}

/* ------------------------------ Funções Auxiliares ------------------------ */

void troca(City *a, City *b) {
    City temp = *a; *a = *b; *b = temp;
}

int compare_x(const void *a, const void *b) {
    City *ca = (City *)a, *cb = (City *)b;
    return (ca->x > cb->x) - (ca->x < cb->x);
}

double custo_tour_matriz(int *tour, int n, double **matriz) {
    double custo = 0.0;
    for (int i = 0; i < n - 1; i++)
        custo += matriz[tour[i]][tour[i + 1]];
    return custo + matriz[tour[n - 1]][tour[0]];
}

/* ------------------------------ Força Bruta ------------------------------- */

void permutacao(City *cities, int comeco, int n, City *melhor_tour, double *custo_min, double **matriz) {
    if (comeco == n - 1) {
        int *tour_indices = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
            tour_indices[i] = cities[i].id - 1;
        double cost = custo_tour_matriz(tour_indices, n, matriz);
        free(tour_indices);
        if (cost < *custo_min) {
            *custo_min = cost;
            memcpy(melhor_tour, cities, n * sizeof(City));
        }
        return;
    }
    for (int i = comeco; i < n; i++) {
        troca(&cities[comeco], &cities[i]);
        permutacao(cities, comeco + 1, n, melhor_tour, custo_min, matriz);
        troca(&cities[comeco], &cities[i]);
    }
}

void permutar(int *vetor, int inicio, int fim, int **matriz, int n, int *melhor_custo, int *visitado, time_t start_time, int tempo_maximo, bool *interrompido) {
    if (*interrompido) return;
    if (time(NULL) - start_time >= tempo_maximo) {
        *interrompido = true;
        return;
    }
    if (inicio == fim) {
        int custo = 0;
        for (int i = 0; i < n - 1; i++)
            custo += matriz[vetor[i]][vetor[i + 1]];
        custo += matriz[vetor[n - 1]][vetor[0]];
        if (custo < *melhor_custo)
            *melhor_custo = custo;
        return;
    }
    for (int i = inicio; i <= fim; i++) {
        int temp = vetor[inicio];
        vetor[inicio] = vetor[i];
        vetor[i] = temp;
        permutar(vetor, inicio + 1, fim, matriz, n, melhor_custo, visitado, start_time, tempo_maximo, interrompido);
        vetor[i] = vetor[inicio];
        vetor[inicio] = temp;
    }
}

double tsp_forca_bruta(int **matriz, int n, int tempo_maximo_segundos) {
    if (n > 11) {
        printf("Instancia muito grande para o algoritmo de forca bruta (n = %d). Abortando.\n", n);
        return -1.0;
    }
    int *vetor = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) vetor[i] = i;
    int melhor_custo = INT_MAX;
    int *visitado = calloc(n, sizeof(int));
    time_t start = time(NULL);
    bool interrompido = false;
    permutar(vetor, 0, n - 1, matriz, n, &melhor_custo, visitado, start, tempo_maximo_segundos, &interrompido);
    free(vetor);
    free(visitado);
    if (interrompido) {
        printf("Tempo limite atingido. Calculo interrompido.\n");
        return -1.0;
    }
    return (double)melhor_custo;
}

/* -------------------------- Divisao e Conquista --------------------------- */


City* forca_bruta_CV(City* cities, int n, double **matriz) {
    City *melhor_tour = malloc(n * sizeof(City));
    double custo_min = INFINITY;
    permutacao(cities, 0, n, melhor_tour, &custo_min, matriz);
    return melhor_tour;
}

City* junta_tours(City *esq, int n_esq, City *dir, int n_dir, double **matriz) {
    int tam = n_esq + n_dir, idx = 0;
    City *resultado = malloc(tam * sizeof(City));
    double melhor = INFINITY;
    int idx_esq = 0, idx_dir = 0, direcao = 0;

    for (int i = 0; i < n_esq; i++) {
        int next_i = (i + 1) % n_esq;
        for (int j = 0; j < n_dir; j++) {
            int next_j = (j + 1) % n_dir;
            double atual = matriz[esq[i].id - 1][esq[next_i].id - 1] + matriz[dir[j].id - 1][dir[next_j].id - 1];
            double nova1 = matriz[esq[i].id - 1][dir[j].id - 1] + matriz[esq[next_i].id - 1][dir[next_j].id - 1];
            double nova2 = matriz[esq[i].id - 1][dir[next_j].id - 1] + matriz[esq[next_i].id - 1][dir[j].id - 1];

            if (nova1 - atual < melhor) {
                melhor = nova1 - atual;
                idx_esq = i; idx_dir = j; direcao = 0;
            }
            if (nova2 - atual < melhor) {
                melhor = nova2 - atual;
                idx_esq = i; idx_dir = j; direcao = 1;
            }
        }
    }

    for (int i = 0; i <= idx_esq; i++) resultado[idx++] = esq[i];
    if (direcao == 0)
        for (int i = 0; i < n_dir; i++) resultado[idx++] = dir[(idx_dir + i + 1) % n_dir];
    else
        for (int i = 0; i < n_dir; i++) resultado[idx++] = dir[(idx_dir - i + n_dir) % n_dir];
    for (int i = idx_esq + 1; i < n_esq; i++) resultado[idx++] = esq[i];

    return resultado;
}

City* divisao_e_conquista(City* cities, int n, double **matriz) {
    if (n <= 3) return forca_bruta_CV(cities, n, matriz);
    int meio = n / 2;
    City *esq = divisao_e_conquista(cities, meio, matriz);
    City *dir = divisao_e_conquista(cities + meio, n - meio, matriz);
    City *resultado = junta_tours(esq, meio, dir, n - meio, matriz);
    free(esq);
    free(dir);
    return resultado;
}

/* ------------------------------- Guloso NN -------------------------------- */

double algoritmo_guloso(double **matriz, int n, int inicio, int *tour) {
    bool *visitadas = calloc(n, sizeof(bool));
    double custo = 0.0;
    int atual = inicio;
    visitadas[atual] = true;
    tour[0] = atual;

    for (int i = 1; i < n; i++) {
        double menor = INFINITY;
        int proxima = -1;
        for (int j = 0; j < n; j++) {
            if (!visitadas[j] && matriz[atual][j] < menor) {
                menor = matriz[atual][j];
                proxima = j;
            }
        }
        visitadas[proxima] = true;
        custo += menor;
        atual = proxima;
        tour[i] = atual;
    }
    custo += matriz[atual][inicio];
    free(visitadas);
    return custo;
}