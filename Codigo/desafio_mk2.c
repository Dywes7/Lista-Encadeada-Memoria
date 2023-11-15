#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_MEM 1000
#define QTD_INIT_PROCESS 10
#define MAX_TAM_PROCESS 50


struct Bloco {
    char status;
    int endereco_inicial;
    int tamanho;
    struct Bloco *prox;
};

struct Bloco *inicializaMemoria(int);
void atualizaMemoria(struct Bloco *, struct Bloco *);
void imprimirLista(struct Bloco *);
void generateRandomAllocation(struct Bloco *);


struct Bloco *inicializaMemoria(int tamanho) {

    struct Bloco *bloco = (struct Bloco*)malloc(sizeof(struct Bloco));
    bloco->status = 'L';
    bloco->endereco_inicial = 0;
    bloco->tamanho = tamanho;
    bloco->prox = NULL;

    return bloco;

}

void atualizaMemoria (struct Bloco *blo_busy, struct Bloco *blo_free) {

    if ((blo_busy->endereco_inicial != 0) && (blo_busy->endereco_inicial +
    blo_busy->tamanho - 1 != TAM_MEM)) {

        int guarda_tam = blo_free->tamanho;

        // BLOCO POSTERIOR
        struct Bloco *bloco = (struct Bloco*)malloc(sizeof(struct Bloco));
        bloco->status = 'L';
        bloco->endereco_inicial = blo_busy->endereco_inicial + blo_busy->tamanho;
        bloco->tamanho = blo_free->tamanho - blo_busy->tamanho - (blo_busy->endereco_inicial - blo_free->endereco_inicial);
        bloco->prox = blo_free->prox;

        // BLOCO ANTERIOR
        blo_free->tamanho = blo_busy->endereco_inicial - blo_free->endereco_inicial;
        blo_free->prox = blo_busy;

        // BLOCO CENTRAL
        blo_busy->prox = bloco;

        // printf("===============================\n\n");
        // printf("BLOCO ANTERIOR\n");
        // printf("blo_free->endereco_inicial: %d\n", blo_free->endereco_inicial);
        // printf("blo_free->tamanho: %d\n", blo_free->tamanho);
        // printf("===============================\n\n");

        // printf("===============================\n\n");
        // printf("BLOCO CENTRAL\n");
        // printf("blo_busy->endereco_inicial: %d\n", blo_busy->endereco_inicial);
        // printf("blo_busy->tamanho: %d\n", blo_busy->tamanho);
        // printf("===============================\n\n");

        // printf("===============================\n\n");
        // printf("BLOCO POSTERIOR\n");
        // printf("bloco->endereco_inicial: %d\n", bloco->endereco_inicial);
        // printf("bloco->tamanho: %d\n", bloco->tamanho);
        // printf("===============================\n\n");

    }

}

void imprimirLista(struct Bloco *memoria) {

    while (memoria != NULL) {
        printf("BLOCO [%c] Inicio: %d, Tamanho: %d\n", memoria->status, memoria->endereco_inicial, memoria->tamanho);
        memoria = memoria->prox;
    }

}

void generateRandomAllocation(struct Bloco *mem) {

    srand(time(NULL));
    int contador = 0;
    int num = 1 + (rand() % QTD_INIT_PROCESS);
    // printf("num: %d\n", num);

    for (int i = 0; i < num; i++) {

        // printf("Estou aqui 0\n");
        struct Bloco *novoBloco = (struct Bloco*)malloc(sizeof(struct Bloco));
        novoBloco->status = 'O';
        novoBloco->tamanho = 1 + (rand() % MAX_TAM_PROCESS);
        
        int var_control = 0;
        do {
            // printf("Estou aqui 1\n");
            novoBloco->endereco_inicial = rand() % TAM_MEM;

            struct Bloco *k;
            for (k = mem; k != NULL; k = k->prox) {

                if (k->status == 'O') {
                // printf("Estou aqui 2\n");    
                continue;

                } else {
                    // printf("Estou aqui 3\n\n");
                    // printf("novoBloco->endereco_inicial: %d\n", novoBloco->endereco_inicial);
                    // printf("novoBloco->tamanho: %d\n", novoBloco->tamanho);
                    // printf("k->tamanho: %d\n", k->tamanho);
                    // printf("k->endereco_inicial: %d\n", k->endereco_inicial);
                    // printf("num: %d\n", num);
                    // printf("Contador: %d\n\n", contador);
                    if ((novoBloco->endereco_inicial >= k->endereco_inicial) &&
                    (novoBloco->endereco_inicial + novoBloco->tamanho - 1) <=
                    k->endereco_inicial + k->tamanho - 1) {
                        
                        // printf("Estou aqui 4\n");
                        contador++;
                        var_control = 1;

                        atualizaMemoria(novoBloco, k);
                        break;

                    }

                }

            }

        } while (var_control != 1);
      
    }


}

int main() {

    struct Bloco *memoria = inicializaMemoria(TAM_MEM);
    generateRandomAllocation(memoria);

    imprimirLista(memoria);
   
    return 0;

}
