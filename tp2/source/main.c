#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "alg.h"

/************************************************************************************************************************************
*                                                  DESCRIÇÃO DO PROJETO                                                             
*                                                                                                                                   
*  Neste trabalho, foram implementados três diferentes algoritmos para a resolução do problema do Caixeiro Viajante, cada um        
*  obedecendo a um paradigma de projeto de algoritmos, sendo eles:                                                                                                                                                                               
*    - Divisão e Conquista                                                                                                             
*    - Algoritmo Guloso
*    -                                                                                                        
*                                                                                                                                  
*  Os algoritmos podem ser avaliados por 20 instâncias diferentes da biblioteca TSPLIB, bastando o usuário passar o
*  nome do arquivo instância como argumento para a execução do projeto.
*
*  Obs: Não é necesário passar o caminho completo, apenas o nome da instância, o próprio código é capaz de buscar o caminho.
*                                                                                                                                   
*  Alunos: Thiago Martins | Pedro Augusto | Jeann Victor | Nicolas Rodrigues                                                        
************************************************************************************************************************************/

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("\nUso: %s <nome_instancia>\n", argv[0]);
        return 1;
    }

    // Garante o caminho correto para o arquivo instância desejado.
    char path[100] = "./instancias/";
    strcat(path, argv[1]);
    int n;
    City *cities_original = ler_instancia(path, &n);
    if (!cities_original) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    double **dist = gerar_matriz_dist(cities_original, n);

    // Cria cópia ordenada para divisão e conquista
    City *cities_ordenadas = malloc(n * sizeof(City));
    memcpy(cities_ordenadas, cities_original, n * sizeof(City));
    qsort(cities_ordenadas, n, sizeof(City), compare_x);

    // Mede o tempo gasto para encontrar uma solução por Divisão e Conquista.
    clock_t inicio = clock();
    City *tour_dc = divisao_e_conquista(cities_ordenadas, n, dist);
    double tempo_dc = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    int *tour_indices = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) tour_indices[i] = tour_dc[i].id - 1;
    double custo_dc = custo_tour_matriz(tour_indices, n, dist);
    free(tour_indices);

    printf("\nAlgoritmo Divisao e Conquista:\nCusto: %.2f\nTempo: %.6f\nTour: ", custo_dc, tempo_dc);
    for (int i = 0; i < n; i++) printf("%d ", tour_dc[i].id);
    printf("%d\n", tour_dc[0].id);

    
    int *tour_g = malloc(n * sizeof(int));
    // Mede o tempo gasto para encontrar uma solução por Algoritmo Guloso.
    inicio = clock();
    double custo_g = algoritmo_guloso(dist, n, 0, tour_g);
    double tempo_g = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    printf("\nResultado do algoritmo Guloso:\nCusto: %.2f\nTempo execucao: %.6f\nTour: ", custo_g, tempo_g);
    for (int i = 0; i < n; i++) printf("%d ", tour_g[i] + 1);
    printf("%d\n", tour_g[0] + 1);

    liberar_matriz(dist, n);
    free(cities_original); free(cities_ordenadas);
    free(tour_dc); free(tour_g);
    return 0;
}
