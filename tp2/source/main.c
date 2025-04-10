#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Struct para armazenar cada cidade.
typedef struct{
    int id;
    double x;
    double y;
}City;

// Função para calcular a distância euclidiana entre dois pontos. √((xb - xa)² + (yb-ya)²)
double calc_dist(City a, City b){
    double dx = pow((b.x - a.x), 2);
    double dy = pow((b.y - a.y), 2);

    return sqrt((dx + dy));
}

/*TODO: Implementação dos 3 algoritmos + matriz de distâncias*/

/************************************************************************************************************************************
*                                                  DESCRIÇÃO DO PROJETO                                                             
*                                                                                                                                   
*  Neste trabalho, foram implementados três diferentes algoritmos para a resolução do problema do Caixeiro Viajante, cada um        
*  obedecendo a um paradigma de projeto de algoritmos, sendo eles:                                                                  
*    - Backtracking                                                                                                                 
*    - Algoritmo Guloso                                                                                                             
*    - Programação Dinâmica                                                                                                         
*                                                                                                                                  
*  Os algoritmos são avaliados podem ser avaliados por 20 instâncias diferentes da biblioteca TSPLIB, bastando o usuário passar o
*  nome do arquivo instância como argumento para a execução do projeto.
*
*  Obs: Não é necesário passar o caminho completo, apenas o nome da instância, o próprio código é capaz de buscar o caminho.
*                                                                                                                                   
*  Alunos: Thiago Martins | Pedro Augusto | Jeann Victor | Nicolas Rodrigues                                                        
************************************************************************************************************************************/


int main(int argc, char** argv){
    if(argc != 2){
        printf("\nA Quantidade de argumentos passada é inválida!");
        return 1;
    }
    
    char full_path[100] = "./instancias/";
    strcat(full_path, argv[1]);

    FILE *arch = fopen(full_path, "r");
    if(!arch){
        printf("\nO arquivo instância não foi aberto corretamente!");
        return 1;
    }

    int dimension = 0;
    char line[128];

    while( fgets(line, sizeof(line), arch)){
        if(strncmp(line, "DIMENSION", 9) == 0){
            sscanf(line, "DIMENSION: %d", &dimension);
        }
        if(strncmp(line, "NODE_COORD_SECTION", 18) == 0){
            break;
        }
    }

    if(dimension == 0){
        printf("\nErro: Dimensão não encontrada!");
        return 1;
    }

    City *cities = malloc(sizeof(City) * dimension);
    for(int i = 0; i < dimension; i++){
        fscanf(arch, "%d %lf %lf", &cities[i].id, &cities[i].x, &cities[i].y);
    }

    fclose(arch);

    free(cities);

    return 0;
}