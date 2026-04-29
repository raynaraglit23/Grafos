/*
 * EP1 - Algoritmo de Dijkstra
 * 
 * Integrantes do Grupo:
 * 1. Raynara Maria Aurélio Coelho
 * 2. Gilberto Magno Souza Vieira
 * 3. Raphael Cabral de Oliveira Figueiredo
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// Função para alocar dinamicamente a matriz de adjacência do grafo
// Usamos tamanho n+1 para poder usar os índices de 1 até n (como no arquivo)
int** criarGrafo(int n) {
    int** grafo = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 1; i <= n; i++) {
        grafo[i] = (int*)malloc((n + 1) * sizeof(int));
        for (int j = 1; j <= n; j++) {
            grafo[i][j] = -1; // -1 representa a ausência de um arco
        }
    }
    return grafo;
}

// Função para liberar a memória alocada para o grafo
void liberarGrafo(int** grafo, int n) {
    for (int i = 1; i <= n; i++) {
        free(grafo[i]);
    }
    free(grafo);
}

// Função para imprimir o caminho no formato exigido
void imprimirCaminho(int* anterior, int inicio, int destino, int custo) {
    if (custo == INT_MAX) {
        printf("Nao ha caminho do vertice %d para o vertice %d.\n", inicio, destino);
        return;
    }

    // Como o array 'anterior' nos dá o caminho de trás para frente, 
    // precisamos armazenar em um array temporário para imprimir na ordem certa
    int* caminho = (int*)malloc(1000 * sizeof(int)); // Tamanho arbitrário seguro
    int atual = destino;
    int tamanho = 0;

    // Constrói o caminho de trás para frente
    while (atual != -1) {
        caminho[tamanho++] = atual;
        atual = anterior[atual];
    }

    printf("\nCaminho minimo do vertice %d para o vertice %d: ", inicio, destino);
    
    // Imprime de frente para trás, pegando os pares (u, v)
    for (int i = tamanho - 1; i > 0; i--) {
        printf("(%d, %d) ", caminho[i], caminho[i - 1]);
    }
    
    printf("\n\nCusto: %d\n", custo);

    free(caminho);
}

// Implementação principal do Algoritmo de Dijkstra
void dijkstra(int** grafo, int n, int origem, int destino) {
    int* distancias = (int*)malloc((n + 1) * sizeof(int));
    int* anterior = (int*)malloc((n + 1) * sizeof(int));
    bool* visitados = (bool*)malloc((n + 1) * sizeof(bool));

    // Inicialização
    for (int i = 1; i <= n; i++) {
        distancias[i] = INT_MAX;
        anterior[i] = -1;
        visitados[i] = false;
    }
    distancias[origem] = 0;

    // Encontra o caminho mais curto para todos os vértices
    for (int cont = 1; cont <= n; cont++) {
        // Encontra o vértice não visitado com a menor distância atual
        int min_dist = INT_MAX;
        int u = -1;

        for (int v = 1; v <= n; v++) {
            if (!visitados[v] && distancias[v] <= min_dist) {
                min_dist = distancias[v];
                u = v;
            }
        }

        // Se o vértice de menor distância for inatingível ou se chegamos ao destino, podemos parar
        if (u == -1 || u == destino) {
            break; 
        }

        visitados[u] = true;

        // Atualiza o valor da distância dos vértices adjacentes ao vértice escolhido
        for (int v = 1; v <= n; v++) {
            // Atualiza apenas se não foi visitado, se existe uma aresta, se a distância de 'u' não é infinita,
            // e se o custo total passando por 'u' é menor do que a distância atual de 'v'
            if (!visitados[v] && grafo[u][v] != -1 && distancias[u] != INT_MAX && 
                distancias[u] + grafo[u][v] < distancias[v]) {
                
                distancias[v] = distancias[u] + grafo[u][v];
                anterior[v] = u;
            }
        }
    }

    // Chama a função para formatar e exibir o resultado
    imprimirCaminho(anterior, origem, destino, distancias[destino]);

    // Liberação de memória
    free(distancias);
    free(anterior);
    free(visitados);
}

int main(int argc, char *argv[]) {
    // Verifica se o arquivo foi passado via linha de comando
    if (argc != 2) {
        printf("Uso correto: %s <nome_do_arquivo.txt>\n", argv[0]);
        return 1;
    }

    // Tenta abrir o arquivo para leitura
    FILE *arquivo = fopen(argv[1], "r");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", argv[1]);
        return 1;
    }

    int n, m, s, t;
    
    // Lê a primeira linha: numero de vértices, arcos, origem e destino
    if (fscanf(arquivo, "%d %d %d %d", &n, &m, &s, &t) != 4) {
        printf("Erro: Formato do arquivo incorreto na primeira linha.\n");
        fclose(arquivo);
        return 1;
    }

    // Cria a matriz de adjacência
    int** grafo = criarGrafo(n);

    // Lê os 'm' arcos do arquivo
    int u, v, c;
    for (int i = 0; i < m; i++) {
        if (fscanf(arquivo, "%d %d %d", &u, &v, &c) == 3) {
            grafo[u][v] = c; // Como é um dígrafo, a atribuição é apenas em um sentido
        }
    }

    fclose(arquivo);

    // Executa o algoritmo
    dijkstra(grafo, n, s, t);

    // Libera a memória alocada para a matriz
    liberarGrafo(grafo, n);

    return 0;
}
