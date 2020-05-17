#include <stdio.h>
#include <stdlib.h>

/*Estrutura para representar as adjacencias do no*/
typedef struct no{
    int destino;
    int distancia;
    struct no* proximo;
}no;

/*Estrutura para representar a lista de adjacencias*/
typedef struct{
    no* head;
}nos;

/*Representa o grafo*/
typedef struct{
    int vertices;
    nos* lista;
}grafo;

/*Criar um novo no de adajcencias*/
no* novoNo(int destino, int distancia){
    no* novo = (no*) malloc(sizeof(no));
    novo->destino = destino;
    novo->distancia = distancia;
    novo->proximo = NULL;
    return novo;
}

/*Criar um grafo com V vertices*/
grafo* criaGrafo(int vertices){
    int i;
    grafo* novo = (grafo*) malloc(sizeof(grafo));
    novo->vertices = vertices;
    novo->lista = (nos*) malloc((vertices+1) * sizeof(nos));
    for (i=1; i<=vertices; i++) {
        novo->lista[i].head = NULL;
    }
    return novo;
}

/*Adicionar um no a um grafo não direcionado*/
void addNo(grafo* grafo, int origem, int destino, int distancia){
    no* novo = novoNo(destino, distancia);
    novo->proximo = grafo->lista[origem].head;
    grafo->lista[origem].head = novo;
}

/*imprimir o grafo*/
void printGrafo(grafo* grafo){
    int i;
    for (i=1; i<= grafo->vertices; i++) {
        no* no = grafo->lista[i].head;
        printf("%d",i);
        while (no) {
            printf("--%d--> %d ", no->distancia, no->destino);
            no = no->proximo;
        }
        printf("\n");
    }
}

/*auxiliar de APrecursiva para calcular o minimo*/
int min(int x, int y){
    if (x>y) {
        return y;
    }
    return x;
}

/*auxiliar de AP onde corro recursivamente*/
void APrecursiva(grafo* grafo,int idNo,int* dfs,int* pai,int* low,int* ap, int time){
    no* no=grafo->lista[idNo].head;
    low[idNo]=dfs[idNo]=time;
    time++;
    while(no){
        if(dfs[no->destino]==0){
            pai[no->destino]=idNo;
            APrecursiva(grafo,no->destino,dfs,pai,low,ap,time);
            low[idNo]= min(low[idNo],low[no->destino]);
            if(dfs[idNo]==1 && low[no->destino]>dfs[idNo])
                ap[idNo]=1;
            if(dfs[idNo]!=1 && low[no->destino]>=dfs[idNo])
                ap[idNo]=1;
        } else if (pai[idNo] != no->destino){
            low[idNo]=min(low[idNo],dfs[no->destino]);
        }
        no = no->proximo;
    }
     
}

/*Verificar se os nos sao servidores*/
void AP(grafo* grafo){
    int* low = (int*)malloc(sizeof(int)*grafo->vertices);
    int* pai = (int*)malloc(sizeof(int)*grafo->vertices);
    int* dfs = (int*)malloc(sizeof(int)*grafo->vertices);
    int* ap = (int*)malloc(sizeof(int)*grafo->vertices);
    int i,flag=0;
    for (i=1; i<=grafo->vertices; i++) {
        pai[i]=-1;
        ap[i]=0;
        dfs[i]=0;
    }
    for(i=1;i<=grafo->vertices;i++){
        if(dfs[i]==0){
            APrecursiva(grafo,i,dfs,pai,low,ap,0);
        }
    }
    for (i=1; i<=grafo->vertices; i++) {
        if(ap[i]==1){
            flag=1;
            printf("AP: %d\n",i);
        }
    }
    if(flag==0){
        printf("\nno server\n");
    }
}

/*ler dados e chamar a funcao*/
int main(int argc, const char * argv[]) {
    int n,destino,distancia;
    grafo* grafo=NULL;
    while(scanf("%d",&n)!=EOF){
        grafo = criaGrafo(n);
        while(n!=0){
            scanf("%d",&n);
            if(n!=0){
                scanf("%d %d",&destino,&distancia);
                addNo(grafo, n, destino,distancia);
                addNo(grafo, destino, n,distancia);
            }
        }
        printf("\n\n");
        printGrafo(grafo);
        printf("\n");
        AP(grafo);
        
    }
    return 0;
}
