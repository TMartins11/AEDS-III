#ifndef ALG_H
#define ALG_H

// Representa uma cidade com coordenadas 2D e identificador único
typedef struct {
    int id;
    double x, y;
} City;

/* -------------------------- Funções de Instância -------------------------- */

// Lê os dados da instância TSPLIB do arquivo
City* ler_instancia(char *filename, int *n);


/* ------------------------- Matriz de Distâncias --------------------------- */

// Gera matriz de distâncias euclidianas entre todas as cidades
double** gerar_matriz_dist(City *cities, int n);

// Libera a matriz de distâncias
void liberar_matriz(double **matriz, int n);


/* ------------------------ Funções Auxiliares ------------------------ */

// Calcula distância euclidiana entre duas cidades
double calc_dist(City a, City b);

// Troca duas cidades de posição em um array
void troca(City *a, City *b);

// Compara cidades pela coordenada X (para qsort)
int compare_x(const void *a, const void *b);

// Calcula o custo de um tour baseado em matriz de distâncias
double custo_tour_matriz(int *tour, int n, double **matriz);


/* ---------------------------- Força Bruta ---------------------------- */

void permutar(int *vetor, int inicio, int fim, int **matriz, int n, int *melhor_custo, int *visitado, time_t start_time, int tempo_maximo, bool *interrompido);

double tsp_forca_bruta(int **matriz, int n, int tempo_maximo_segundos);


/* ------------------------- Divisão e Conquista ------------------------- */

City* forca_bruta_CV(City* cities, int n, double **matriz);

// Junta dois subtours minimizando o custo total ao unir as bordas
City* junta_tours(City *esq, int n_esq, City *dir, int n_dir, double **matriz);

// Resolve o TSP recursivamente usando divisão e conquista
City* divisao_e_conquista(City* cities, int n, double **matriz);


/* --------------------------- Guloso (NN) --------------------------- */

// Implementação do algoritmo guloso (nearest neighbor)
double algoritmo_guloso(double **matriz, int n, int inicio, int *tour);

#endif