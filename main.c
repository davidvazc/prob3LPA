#include <stdio.h>
#include <stdlib.h>

/*Estrutura para representar as adjacencias do no*/
typedef struct no
{
    int destino;
    int distancia;
    struct no *proximo;
} no;

/*Estrutura para representar a lista de adjacencias*/
typedef struct
{
    no *head;
} nos;

/*Representa o grafo*/
typedef struct
{
    int vertices;
    nos *lista;
} grafo;

/*Criar um novo no de adajcencias*/
no *novoNo(int destino, int distancia)
{
    no *novo = (no *)malloc(sizeof(no));
    novo->destino = destino;
    novo->distancia = distancia;
    novo->proximo = NULL;
    return novo;
}

/*Criar um grafo com V vertices*/
grafo *criaGrafo(int vertices)
{
    int i;
    grafo *novo = (grafo *)malloc(sizeof(grafo));
    novo->vertices = vertices;
    novo->lista = (nos *)malloc((vertices + 1) * sizeof(nos));
    for (i = 1; i <= vertices; i++)
    {
        novo->lista[i].head = NULL;
    }
    return novo;
}

/*Adicionar um no a um grafo não direcionado*/
void addNo(grafo *grafo, int origem, int destino, int distancia)
{
    no *novo = novoNo(destino, distancia);
    novo->proximo = grafo->lista[origem].head;
    grafo->lista[origem].head = novo;
}

/*imprimir o grafo*/
void printGrafo(grafo *grafo)
{
    int i;
    for (i = 1; i <= grafo->vertices; i++)
    {
        no *no = grafo->lista[i].head;
        while (no)
        {
            printf("%d --%d--> %d \t", i, no->distancia, no->destino);
            no = no->proximo;
        }
        printf("\n");
        free(no);
    }
}

/*auxiliar de APrecursiva para calcular o minimo*/
int min(int x, int y)
{
    if (x > y)
    {
        return y;
    }
    return x;
}

/*devolve o mais perto*/
int maisProximo(grafo *grafo, int i, int Q[])
{
    int menor = 1000000;
    int resp = 0;
    no *no = grafo->lista[i].head;
    while (no)
    {
        if (Q[no->destino] == 0)
        {
            if (no->distancia < menor)
            {
                menor = no->distancia;
                resp = no->destino;
            }
            no = no->proximo;
        }
    }
    free(no);
    printf("Mais proximo foi: %d", resp);
    return resp;
}

/*dijkstra*/
int dijkstra(grafo *grafo, int s, int t)
{
    int V = grafo->vertices;

    int Q[V];
    int d[V];
    int i, u, v;
    for (i = 1; i < V; i++)
    {
        if (i != s)
        {
            d[i] = 1000000;
        }
        Q[i] = 0;
    }
    d[s] = 0;
    while (Q[t] == 0)
    {
        u = maisProximo(grafo, s, Q);
        Q[u] = 1;
        if (u == t)
            break;
        no *no = grafo->lista[u].head;
        v = no->destino;
        while (no)
        {
            if (d[v] > d[u] + no->distancia)
                d[v] = d[u] + no->distancia;
            no = no->proximo;
        }
    }
    printf("\n-->%d\n", d[t]);
    return d[t];
}

/*Floyd-warshall*/
void FW(grafo *grafo, int **D)
{
    int i, k, j;
    for (i = 1; i <= grafo->vertices; i++)
    {
        for (j = 1; j <= grafo->vertices; j++)
        {
            //            printf("i: %d, j:%d\n",i,j);
            if (i == j)
            {
                D[i][j] = 0;
            }
            else
            {
                D[i][j] = 100000;
            }
        }
    }
    for (i = 1; i <= grafo->vertices; i++)
    {
        no *no = grafo->lista[i].head;
        while (no)
        {
            //            printf("i: %d, no->destino: %d, no->distancia: %d\n",i,no->destino,no->distancia);
            D[i][no->destino] = no->distancia;
            no = no->proximo;
        }
        free(no);
    }
    for (k = 1; k <= grafo->vertices; k++)
    {
        for (i = 1; i <= grafo->vertices; i++)
        {
            for (j = 1; j <= grafo->vertices; j++)
            {
                if (D[i][j] > D[i][k] + D[k][j])
                {
                    //                    printf("[%d][%D]: %d, [%d][%D]: %d, [%d][%D]: %d\n",i,j,D[i][j],i,k,D[i][k],k,j,D[k][j]);
                    D[i][j] = D[i][k] + D[k][j];
                }
            }
        }
    }
}

/*auxiliar de AP onde corro recursivamente*/
void APrecursiva(grafo *grafo, int idNo, int *dfs, int *pai, int *low, int *ap, int time)
{
    no *no = grafo->lista[idNo].head;
    low[idNo] = dfs[idNo] = time;
    time++;
    while (no)
    {
        if (dfs[no->destino] == 0)
        {
            pai[no->destino] = idNo;
            APrecursiva(grafo, no->destino, dfs, pai, low, ap, time);
            low[idNo] = min(low[idNo], low[no->destino]);
            if (dfs[idNo] == 1 && low[no->destino] > dfs[idNo])
                ap[idNo] = 1;
            if (dfs[idNo] != 1 && low[no->destino] >= dfs[idNo])
                ap[idNo] = 1;
        }
        else if (pai[idNo] != no->destino)
        {
            low[idNo] = min(low[idNo], dfs[no->destino]);
        }
        no = no->proximo;
    }
    free(no);
}

/*Verificar se os nos sao servidores*/
void AP(grafo *grafo, int **D)
{
    int *low = (int *)malloc(sizeof(int) * grafo->vertices);
    int *pai = (int *)malloc(sizeof(int) * grafo->vertices);
    int *dfs = (int *)malloc(sizeof(int) * grafo->vertices);
    int *ap = (int *)malloc(sizeof(int) * grafo->vertices);
    int i, j, resp1 = 0, resp2 = 0, resp3 = 0;
    for (i = 1; i <= grafo->vertices; i++)
    {
        pai[i] = -1;
        ap[i] = 0;
        dfs[i] = 0;
    }
    for (i = 1; i <= grafo->vertices; i++)
    {
        if (dfs[i] == 0)
        {
            APrecursiva(grafo, i, dfs, pai, low, ap, 0);
        }
    }
    for (i = 1; i <= grafo->vertices; i++)
    {
        if (ap[i] == 1)
        {
            resp1++;
        }
    }

    for (i = 1; i <= grafo->vertices; i++)
    {
        if (ap[i] == 1)
        {
            for (j = i + 1; j <= grafo->vertices; j++)
            {
                if (ap[j] == 1)
                {
                    //                    printf("do %d ao %d da: %d\n",i,j,D[i][j]);
                    resp2 = resp2 + D[i][j];
                }
            }
        }
    }

    if (resp1 == 0)
    {
        printf("no server\n");
    }
    else if (resp1 == 1)
    {
        printf("1 0 0\n");
    }
    else
    {
        printf("%d %d %d\n", resp1, resp2, resp3);
    }
    free(low);
    free(pai);
    free(dfs);
    free(ap);
}

/*ler dados e chamar a funcao*/
int main(int argc, const char *argv[])
{
    int n, destino, distancia, i;
    grafo *grafo = NULL;
    int **D;
    while (scanf("%d", &n) != EOF)
    {
        grafo = criaGrafo(n);
        while (n != 0)
        {
            scanf("%d", &n);
            if (n != 0)
            {
                scanf("%d %d", &destino, &distancia);
                addNo(grafo, n, destino, distancia);
                addNo(grafo, destino, n, distancia);
            }
        }

        D = (int **)malloc((1 + grafo->vertices) * sizeof(int *));
        for (i = 0; i <= grafo->vertices; i++)
            D[i] = (int *)malloc((1 + grafo->vertices) * sizeof(int));
        FW(grafo, D);

        //        printf("\n\n");
        //        printGrafo(grafo);
        //        printf("\n");
        AP(grafo, D);

        free(D);
    }
    free(grafo);
    return 0;
}
