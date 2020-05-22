#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

int D[SHRT_MAX][SHRT_MAX];

/*Estrutura para representar as adjacencias do no*/
typedef struct no{
    int origem;
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

/*estrutura para reprensentar union-find*/
struct set{
    int pai;
    int rank;
}set;
/*Criar um novo no de adajcencias*/
no* novoNo(int origem,int destino, int distancia){
    no* novo = (no*) malloc(sizeof(no));
    novo->origem = origem;
    novo->destino = destino;
    novo->distancia = distancia;
    novo->proximo = NULL;
    return novo;
}

/*Criar um grafo com V vertices*/
grafo* criaGrafo(int vertices){
    int i;
    grafo* novo = (grafo*) malloc(sizeof(grafo));
    novo->lista= (nos*) malloc((vertices+1) * sizeof(nos));
    novo->vertices = vertices;
    novo->lista[0].head = NULL;
    for (i=1; i<=vertices; i++) {
        novo->lista[i].head = NULL;
    }
    return novo;
}

/*Adicionar um no a um grafo não direcionado*/
void addNo(grafo* grafo, int origem, int destino, int distancia){
    no* novo = novoNo(origem,destino, distancia);
    novo->proximo = grafo->lista[origem].head;
    grafo->lista[origem].head = novo;
}

/* function to swap data of two nodes a and b*/
void swap(no *a, no *b)
{
    no * temp = (no*) malloc(sizeof(no));
    temp->distancia=a->distancia;
    temp->destino=a->destino;
    temp->origem=a->origem;
    a->distancia = b->distancia;
    a->destino = b->destino;
    a->origem = b->origem;
    b->distancia = temp->distancia;
    b->destino = temp->destino;
    b->origem = temp->origem;
    free(temp);
}

/* Bubble sort the given linked list */
void bubbleSort(no *start)
{
    int swapped;
    no *ptr1;
    no *lptr = NULL;
    
    /* Checking for empty list */
    if (start == NULL)
        return;
    
    do
    {
        swapped = 0;
        ptr1 = start;
        
        while (ptr1->proximo != lptr)
        {
            if (ptr1->distancia > ptr1->proximo->distancia)
            {
                swap(ptr1, ptr1->proximo);
                swapped = 1;
            }
            ptr1 = ptr1->proximo;
        }
        lptr = ptr1;
    }
    while (swapped);
}

/*imprimir o grafo*/
void printGrafo(grafo* grafo){
    int i;
    no* no = NULL;
    for (i=1; i<= grafo->vertices; i++) {
        no = grafo->lista[i].head;
        while (no) {
            printf("%d --%d--> %d \t",i, no->distancia, no->destino);
            no = no->proximo;
        }
        printf("\n");
        free(no);
    }
    
}

/*imprimir o grafo*/
void printLista(no* lista){
    no* no = lista;
    while (no) {
        printf("%d --%d--> %d \n",no->origem, no->distancia, no->destino);
        no = no->proximo;
    }
    printf("\n");
    free(no);
}

/*DFS auxiliar*/
int DFSaux(grafo* g, int* visitado, int v, int ap[],int t,int primeiro, int respAux){
    int resp=0;
    no* no = g->lista[v].head;
    visitado[v]=1;
    /*printf("----------V:%d[%d]--------\n",v,t++);*/
    while (no) {
        if(visitado[no->destino]==0){
            if(ap[v]==1){
                /*printf("1:V:%d[%d]\t %d -- D:%d d:%d--->%d\n",v,t,no->origem,D[v][no->destino],no->distancia,no->destino);*/
                respAux = 0;
                primeiro =1;
            }
            respAux += no->distancia;
            if(ap[no->destino]==1){
                if(primeiro ==1){
                    /*printf("2:V:%d[%d]-- D:%d d:%d--->%d\n",v,t,D[v][no->destino],no->distancia,no->destino);*/
                    resp+=respAux;
                }
            }
            //            printf("\nArvore %d T=%d\tEntrou para %d--%d-->%d\n",v,t,no->origem,no->distancia,no->destino);
            resp+=DFSaux(g, visitado, no->destino, ap,t,primeiro,respAux);
        }
        no = no->proximo;
    }
    /*printf("\nrespAux:%d\tresp: %d\n",respAux,resp);*/
    return resp;
}

/*DFS MAIN*/
int DFS(grafo* g, int ap[]){
    int i,resp=0,primeiro=0;
    int visitado[g->vertices+1];
    
    for (i=1; i<=g->vertices; i++) {
        visitado[i]=0;
    }
    
    for(i=1;i<=g->vertices;i++){
        if(visitado[i]==0){
            resp+=DFSaux(g, visitado, i, ap,1,primeiro,0);
        }
    }
    
    return resp;
}

/*Floyd-warshall*/
int FW(grafo* grafo,int ap[], int servers){
    int i,k,j,resp2=0,primeiro=0,ultimo=0;
    no* no = NULL;
    for (i=1; i<=grafo->vertices; i++) {
        for (j=1; j<=grafo->vertices; j++) {
            //            printf("i: %d, j:%d\n",i,j);
            if(i==j){
                D[i][j]=0;
            }
            else{
                D[i][j]=SHRT_MAX;
            }
        }
    }
    for (i=1; i<=grafo->vertices; i++) {
        no = grafo->lista[i].head;
        while (no) {
            //            printf("i: %d, no->destino: %d, no->distancia: %d\n",i,no->destino,no->distancia);
            D[i][no->destino]=no->distancia;
            no = no->proximo;
        }
        free(no);
    }
    for (k=1; k<=grafo->vertices; k++) {
        for (i=1; i<=grafo->vertices; i++) {
            for (j=1; j<=grafo->vertices; j++) {
                if (D[i][j] > D[i][k] + D[k][j]){
                    //                    printf("[%d][%D]: %d, [%d][%D]: %d, [%d][%D]: %d\n",i,j,D[i][j],i,k,D[i][k],k,j,D[k][j]);
                    D[i][j] = D[i][k] + D[k][j];
                }
            }
        }
    }
    for (i=1; i<=grafo->vertices; i++) {
        if(ap[i]==1){
            if(ultimo<i)
                ultimo=i;
            for (j=i+1; j<=grafo->vertices; j++) {
                if(ap[j]==1){
                    /*                    printf("0: do %d ao %d da: %d\n",i,j,D[i][j]);*/
                    if(D[i][j]<1000){
                        /*                        printf("1: do %d ao %d da: %d\n",i,j,D[i][j]);*/
                        resp2=resp2+D[i][j];
                        if(primeiro==0){
                            primeiro=i;
                        }
                    } else{
                        /*                        printf("2: do %d ao %d da: %d, %d\n",i,j,D[i][j],SHRT_MAX);*/
                        resp2=resp2+D[primeiro][ultimo];
                        primeiro=0;
                    }
                    break;
                }
            }
        }
    }
    if(D[primeiro][ultimo]<SHRT_MAX && servers!=2){
        resp2=resp2+D[primeiro][ultimo];
        /*        printf("3: do %d ao %d da: %d\n",primeiro,ultimo,D[primeiro][ultimo]);*/
    }
    return resp2;
}

/*kruskal find*/
int find(struct set* subset, int a){
    if(subset[a].pai != a)
        subset[a].pai = find(subset,subset[a].pai);
    return subset[a].pai;
}

/*kruskal link*/
void Link(struct set* subset, int a, int b){
    if(subset[a].rank > subset[b].rank)
        subset[b].pai=a;
    else
        subset[a].pai=b;
    if(subset[a].rank == subset[b].rank)
        subset[b].rank++;
}

/*kruskal union*/
void Union(struct set* subset, int a, int b){
    Link(subset,find(subset, a),find(subset, b));
}

/*kruskal main*/
grafo* Kruskal(grafo* graf){
    int V = graf->vertices+1;
    int v, a, b;
    no* lista = (no*) malloc(sizeof(no));
    no* noAux, *temp, *novo;
    struct set *sets =(struct set*) malloc(sizeof(struct set) * V +1);
    grafo* G=criaGrafo(V-1);
    lista->proximo=NULL;
    
    for(v=1; v<=graf->vertices;v++){
        sets[v].pai = v;
        sets[v].rank = 0;
    }
    temp=lista;
    for (v=1; v<=graf->vertices; v++) {
        noAux = graf->lista[v].head;
        while (noAux) {
            /*            printf("adicionou %d--%d-->%d\n",v,noAux->distancia,noAux->destino);
                        printf("adicionou %d--%d-->%d\n",origem,destino,distancia);*/
            novo = novoNo(noAux->origem,noAux->destino, noAux->distancia);
            temp->proximo =novo;
            temp = temp->proximo;
            
            noAux = noAux->proximo;
        }
    }
    
    /*printf("\nGRAFO ORDENADO:\n");*/
    bubbleSort(lista->proximo);
    /*printLista(lista->proximo);*/
    
    noAux = lista->proximo;
    while (noAux) {
        a = find(sets, noAux->origem);
        b = find(sets, noAux->destino);
        if(a != b){
            addNo(G, noAux->origem, noAux->destino, noAux->distancia);
            addNo(G, noAux->destino,noAux->origem, noAux->distancia);
            Union(sets, a, b);
        }
        noAux = noAux->proximo;
    }
    
    free(sets);
    free(lista);
    return G;
}

/*auxiliar de AP onde corro recursivamente*/
void AP(grafo* grafo,int v,int* dfs,int* pai,int* low,int* ap, int t){
    no* no=grafo->lista[v].head;
    int w, filho=0;
    low[v]=dfs[v]=t;
    t++;
    while(no){
        w=no->destino;
        if(!dfs[w]){
            filho++;
            pai[w]=v;
            AP(grafo,w,dfs,pai,low,ap,t);
            low[v]= fmin(low[v],low[w]);
            if(dfs[v]==1 && filho>1)
                ap[v]=1;
            if(dfs[v]!=1 && low[w]>=dfs[v])
                ap[v]=1;
        } else if (pai[v] != w){
            low[v]=fmin(low[v],dfs[w]);
        }
        no = no->proximo;
    }
}

/*Verificar se os nos sao servidores*/
void APGeral(grafo* g, grafo* Gkrus){
    int low[1+g->vertices], pai[1+g->vertices], dfs[1+g->vertices], ap[1+g->vertices];
    int i,servidores=0,resp2=0, resp3=0;
    grafo* gAux=NULL;
    for (i=0; i<=g->vertices; i++) {
        pai[i]=0;
        ap[i]=0;
        dfs[i]=0;
    }
    for(i=0;i<=g->vertices;i++){
        if(dfs[i]==0){
            AP(g,i,dfs,pai,low,ap,1);
        }
    }
    for (i=0; i<=g->vertices; i++) {
        if(ap[i]==1){
            servidores++;
        }
    }
    
    if(servidores==0){
        printf("no server\n");
    }else if(servidores==1){
        printf("1 0 0\n");
    }else if(servidores==2){
        resp2=FW(g,ap,servidores);
        printf("%d %d %d\n",servidores,resp2,resp2);
    }else{
        resp2=FW(g,ap,servidores);
        gAux=Kruskal(Gkrus);
        resp3=DFS(gAux,ap);
        printf("%d %d %d\n",servidores,resp2,resp3);
        /*printf("\nkruskall:\n");
        printGrafo(gAux);
        printf("\nresultado kruskal: %d\n",resp3);*/
    }
    free(gAux);
}

void freeList(struct no* head){
    no* tmp;
    while (head != NULL){
        tmp = head;
        head = head->proximo;
        free(tmp);
    }
}

/*ler dados e chamar a funcao*/
int main(int argc, const char * argv[]) {
    int n,destino,distancia,i;
    grafo* g=NULL, *gkrus=NULL;
    while(scanf("%d",&n)!=EOF){
        g = criaGrafo(n);
        gkrus = criaGrafo(n);
        while(n>0){
            scanf("%d",&n);
            if(n!=0){
                scanf("%d %d",&destino,&distancia);
                addNo(g, n, destino,distancia);
                addNo(g, destino, n,distancia);
                addNo(gkrus, n, destino, distancia);
            }
        }
        /*        printf("\n\n");
                printGrafo(g);
               printf("\n");*/
        APGeral(g, gkrus);
        
        
        
        for (i=0; i<= g->vertices; i++) {
            freeList(g->lista[i].head);
            freeList(gkrus->lista[i].head);
        }
        free(g->lista);
        free(g);
        
        free(gkrus->lista);
        free(gkrus);
        
    }
    
    return 0;
}
