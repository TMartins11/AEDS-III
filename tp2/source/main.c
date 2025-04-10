#include <stdio.h>
#include <stdlib.h>

/*TODO: Implementação dos 3 algoritmos + leitura dos arquivos instância.*/

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

    char path[] = "./instancias/";
    
    char full_path[50];
    int i = 0, j = 0;

    while(path[i] != '/0'){
        full_path[j++] = path[i++];
    }

    i = 0;
    while(argv[1][i] != '/0'){
        full_path[j++] = argv[1][i++];
    }

    full_path[j] = '/0';
    
    FILE *arch = fopen(full_path, "r");
    if(!arch){
        printf("\nO arquivo instância não foi aberto corretamente!");
        return 1;
    }

    fclose(arch);

    return 0;
}