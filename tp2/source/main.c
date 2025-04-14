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
*    - Força Bruta (limitada a n <= 11)                                                                                                        
*                                                                                                                                  
*  Os algoritmos podem ser avaliados por 20 instâncias diferentes da biblioteca TSPLIB, bastando o usuário passar o
*  nome do arquivo da instância como argumento para a execução do projeto.
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

    // Caminho do arquivo da instância
    char path[100] = "./instancias/";
    strcat(path, argv[1]);

    int n;
    City *cities_original = ler_instancia(path, &n);
    if (!cities_original) {
        printf("Erro ao abrir o arquivo da instância.\n");
        return 1;
    }

    double **dist = gerar_matriz_dist(cities_original, n);

    // Cria cópia ordenada por x para o algoritmo de divisão e conquista
    City *cities_ordenadas = malloc(n * sizeof(City));
    memcpy(cities_ordenadas, cities_original, n * sizeof(City));
    qsort(cities_ordenadas, n, sizeof(City), compare_x);

    // Divisão e Conquista
    clock_t inicio = clock();
    City *tour_dc = divisao_e_conquista(cities_ordenadas, n, dist);
    double tempo_dc = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    int *indices_dc = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) indices_dc[i] = tour_dc[i].id - 1;
    double custo_dc = custo_tour_matriz(indices_dc, n, dist);
    free(indices_dc);

    printf("\nAlgoritmo Divisão e Conquista:\nCusto: %.2f\nTempo: %.6f\nTour: ", custo_dc, tempo_dc);
    for (int i = 0; i < n; i++) printf("%d ", tour_dc[i].id);
    printf("%d\n", tour_dc[0].id);

    // Algoritmo Guloso
    int *tour_g = malloc(n * sizeof(int));
    inicio = clock();
    double custo_g = algoritmo_guloso(dist, n, 0, tour_g);
    double tempo_g = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    printf("\nAlgoritmo Guloso:\nCusto: %.2f\nTempo: %.6f\nTour: ", custo_g, tempo_g);
    for (int i = 0; i < n; i++) printf("%d ", tour_g[i] + 1);
    printf("%d\n", tour_g[0] + 1);

    // Algoritmo Força Bruta (opcional, apenas para n <= 11)
    double custo_fb = -1.0, tempo_fb = 0.0;
    if (n <= 11) {
        int **dist_int = malloc(n * sizeof(int*));
        for (int i = 0; i < n; i++) {
            dist_int[i] = malloc(n * sizeof(int));
            for (int j = 0; j < n; j++)
                dist_int[i][j] = (int)(dist[i][j] + 0.5);
        }

        inicio = clock();
        custo_fb = tsp_forca_bruta(dist_int, n, 10);
        tempo_fb = (double)(clock() - inicio) / CLOCKS_PER_SEC;

        if (custo_fb > 0)
            printf("\nAlgoritmo Força Bruta:\nCusto: %.2f\nTempo: %.6f\n", custo_fb, tempo_fb);

        for (int i = 0; i < n; i++) free(dist_int[i]);
        free(dist_int);
    }else{
        // Com 11 cidades o número de permutações necessárias já ultrapassa 39 milhões...
        printf("\nForça Bruta:\n");
        printf("\nÉ impossível computar a solução em força bruta, visto que o número de cidades é maior ou igual a 11!\n");
    }

    // Liberação de memória
    liberar_matriz(dist, n);
    free(cities_original);
    free(cities_ordenadas);
    free(tour_dc);
    free(tour_g);

    // Escrita dos resultados
    FILE *results = fopen("resultados.csv", "a");
    if (results) {
        fprintf(results, "%s,divisao_e_conquista,%.6f,%.2f\n", argv[1], tempo_dc, custo_dc);
        fprintf(results, "%s,guloso,%.6f,%.2f\n", argv[1], tempo_g, custo_g);
        if (custo_fb > 0)
            fprintf(results, "%s,forca_bruta,%.6f,%.2f\n", argv[1], tempo_fb, custo_fb);
        fclose(results);
    } else {
        printf("Erro ao abrir arquivo de resultados.\n");
    }

    return 0;
}