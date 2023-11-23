#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_MEM 1000
#define QTD_INIT_PROCESS 5
#define MAX_TAM_PROCESS 50


struct Bloco {

    char status;
    int endereco_inicial;
    int tamanho;
    struct Bloco *prox;

};

struct Bloco *inicializaMemoria(int);
void atualizaMemoriaInit(struct Bloco *, struct Bloco *);
void imprimirLista(struct Bloco *);
void generateRandomAllocation(struct Bloco *);
void atualizaMemoriaDefinitive(struct Bloco *, struct Bloco *, struct Bloco *);
struct Bloco *adicionarRemoverProcesso(struct Bloco *, int);
void desalocarProcesso(struct Bloco *memoria);


struct Bloco *inicializaMemoria(int tamanho) {

    struct Bloco *bloco = (struct Bloco*)malloc(sizeof(struct Bloco));
    bloco->status = 'L';
    bloco->endereco_inicial = 0;
    bloco->tamanho = tamanho;
    bloco->prox = NULL;

    return bloco;

}

void atualizaMemoriaInit (struct Bloco *blo_busy, struct Bloco *blo_free) {

    if ((blo_busy->endereco_inicial != 0) && (blo_busy->endereco_inicial +
    blo_busy->tamanho - 1 != (TAM_MEM - 1))) {

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

    }

}

void imprimirLista(struct Bloco *memoria) {

    printf("\n****MEMORIA****\n");
    while (memoria != NULL) {

        printf("BLOCO [%c] Inicio: %d, Tamanho: %d.\n", memoria->status, memoria->endereco_inicial, memoria->tamanho);
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

                        atualizaMemoriaInit(novoBloco, k);
                        break;

                    }

                }

            }

        } while (var_control != 1);
      
    }


}

void atualizaMemoriaDefinitive(struct Bloco *blo_busy, struct Bloco *blo_free, struct Bloco *blo_init) {

    // BUSCANDO BLOCO ANTERIOR AO NOVO BLOCO CASO O ENDERECO INCIAL DO NOVO BLOCO FOR DIFERENTE DE ZERO
    if (blo_busy->endereco_inicial != 0) {

    // ACHAR BLOCO ANTERIOR AO NOVO BLOCO CRIADO
    struct Bloco *blo_before = blo_init;

    // printf("\nBLOCO (Before): %p, [%c] Inicio: %d, Tamanho: %d, Prox_end: %p\n", blo_before, blo_before->status, blo_before->endereco_inicial, blo_before->tamanho, blo_before->prox);
    // printf("\nBLOCO (Free): %p, [%c] Inicio: %d, Tamanho: %d, Prox_end: %p\n", blo_free, blo_free->status, blo_free->endereco_inicial, blo_free->tamanho, blo_free->prox);

    while (blo_before->prox != blo_free) {
        
        blo_before = blo_before->prox;

    }

    // BLOCO ANTERIOIR RECEBE O PRÓXIMO (NOVO BLOCO CRIADO)
    blo_before->prox = blo_busy;

    }

   
    // CASO PARA NOVO BLOCO TEM TAMANHO DIFERENTE DO ESPAÇO LIVRE
    if (blo_busy->tamanho != blo_free->tamanho) {

        // blo_busy->endereco_inicial = blo_free->endereco_inicial;
        blo_busy->prox = blo_free;

        blo_free->endereco_inicial = blo_busy->endereco_inicial + blo_busy->tamanho;
        blo_free->tamanho = blo_free->tamanho - blo_busy->tamanho;

        // // SE NOVO BLOCO INICIAR NO ENDERECO 0, ENTAO TROCAR DE POSICAO COM PRIMEIRO MEMBRO DA LISTA ENCADEADA
        // if (blo_busy->endereco_inicial == 0) {

        //     *init_mem = blo_busy;

        // }

    } else {    // TAMANHO DO NOVO BLOCO = AO TAMANHO DO BLOCO LIVRE

        blo_busy->prox = blo_free->prox;

        free(blo_free);


    }



}

struct Bloco *adicionarRemoverProcesso(struct Bloco *mem, int alg) {

    int tam_process;
    struct Bloco *endereco_retorno = mem;
    int var_control = 0;

    printf("\nOPCOES ALOCACAO/DESALOCAO...\n");
    printf("Digite um tamanho para ALOCAR um processo.\n");
    printf("Digite 0 para encerrar.\n");
    printf("Digite -1 para DESALOCAR um processo.\n");
    printf("Valor: ");
    scanf("%d", &tam_process);

    if (tam_process == 0) {

        printf("\nPrograma Encerrado.\n");
        exit(0);

    } else if (tam_process == -1) {

        desalocarProcesso(mem);
        // imprimirLista(mem);

    } else if (tam_process < -1) {
        
        printf("\nTamanho de processo invalido.\n");
        
        } else {

        // CRIAR NOVO BLOCO

        struct Bloco *newBloco = (struct Bloco*)malloc(sizeof(struct Bloco));
        newBloco->status = 'O'; 
        newBloco->tamanho = tam_process;

        // ALGORITMO PARA FIRST-FIT
        if (alg == 0) {
            
            struct Bloco *i;
            for (i = mem; i != NULL; i = i->prox) {
            
                if (i->status == 'L' && i->tamanho >= tam_process) {
                    
                    var_control++;
                    newBloco->endereco_inicial = i->endereco_inicial;
                    atualizaMemoriaDefinitive(newBloco, i, mem);

                    if (newBloco->endereco_inicial == 0) {

                        endereco_retorno = newBloco;

                    }

                    break;

                }

            }

            if (var_control == 0) {
                
                printf("\nBlocos de memoria insuficiente para alocacao :(\n");

            }


        } else {    // ALGORITMO PARA BEST-FIT

            struct Bloco *i;
            int menor_tam;
            // INICIALIZAR J com algum tamanho de um bloco livre
            for (i = mem; i != NULL; i = i->prox) {

                if (i->status == 'L' && i->tamanho >= tam_process) {

                    menor_tam = i->tamanho;
                    break;

                }

            }

            // PROCURAR MENOR TAMANHO DE BLOCO LIVRE COMPATÍVEL COM O NOVO BLOCO
            for (i = mem; i != NULL; i = i->prox) {

                if (i->status == 'L' && i->tamanho >= tam_process) {

                    if (i->tamanho < menor_tam) {

                        menor_tam = i->tamanho;

                    }

                } 

            }

            for (i = mem; i != NULL; i = i->prox) { 

                if (i->status == 'L' && i->tamanho == menor_tam) {

                    var_control++;
                    newBloco->endereco_inicial = i->endereco_inicial;
                    atualizaMemoriaDefinitive(newBloco, i, mem);

                    if (newBloco->endereco_inicial == 0) {

                        endereco_retorno = newBloco;

                    }

                    break;

                }          

            }

             if (var_control == 0) {
                
                printf("Blocos de memoria insuficiente para alocacao :(\n");

            }
           

        } 

    }

    return endereco_retorno;

}

void desalocarProcesso(struct Bloco *memoria) {

    int init_address;
    printf("Digite o endereco inicial do processo a ser removido: ");
    scanf("%d", &init_address);

    struct Bloco *i;
    struct Bloco *blo_before = memoria;
    for (i = memoria; i != NULL; i = i->prox) {

        if (init_address == i->endereco_inicial && i->status == 'L') {

            printf("\nENDERECO DE PROCESSO INVALIDO.\n");
            break;

        }

        if (init_address == i->endereco_inicial && i->status == 'O') {

            // BUSCANDO BLOCO ANTERIOR AO BLOCO A SER REMOVIDO
            if (i->endereco_inicial != 0 && (i->endereco_inicial + i->tamanho) != TAM_MEM) {

                while (blo_before->prox != i) {
                    
                    blo_before = blo_before->prox;

                }

                // BLOCO OCUPADO NO MEIO DE DOIS LIVRES
                if (blo_before->status == 'L' && i->prox->status == 'L') {

                    blo_before->tamanho = blo_before->tamanho + i->tamanho + i->prox->tamanho;
                    blo_before->prox = i->prox->prox;
                    
                    free(i->prox);
                    free(i);
                    break;

                }

                // BLOCO BEFORE = LIVRE, BLOCO AFTER = OCUPADO
                if (blo_before->status == 'L' && i->prox->status == 'O') {

                    blo_before->tamanho = blo_before->tamanho + i->tamanho;
                    blo_before->prox = i->prox;
                    
                    free(i);
                    break;

                }

                // BLOCO BEFORE = OCUPADO, BLOCO AFTER = LIVRE
                if (blo_before->status == 'O' && i->prox->status == 'L') {

                    blo_before->prox = i->prox;
                    i->prox->endereco_inicial = i->endereco_inicial;
                    i->prox->tamanho += i->tamanho; 
                    
                    free(i);
                    break;

                }

                if (blo_before->status == 'O' && i->prox->status == 'O') {

                    i->status = 'L'; 
                    
                    break;

                }
    
            }

            // BLOCO A SER REMOVIDO ESTÁ NO INÍCIO DA LISTA
            if (i->endereco_inicial == 0) {

                // CASO BLOCO A FRENTE DO OCUPADO ESTÁ LIVRE
                if (i->prox->status == 'L') {
                  
                    struct Bloco *guarda_endereco = i->prox;

                    i->status = 'L';
                    i->tamanho = i->tamanho + i->prox->tamanho;
                    i->prox = i->prox->prox;
                   
                    free(guarda_endereco);
                    break;
                    
                }

                // CASO BLOCO A FRENTE DO OCUPADO ESTÁ OCUPADO
                if (i->prox->status == 'O') {

                    i->status = 'L';
                    break;

                }



            }

            // BLOCO A SER REMOVIDO ESTÁ NO FIM DA LISTA
            if ((i->endereco_inicial + i->tamanho) == TAM_MEM) {

                // BUSCANDO BLOCO ANTERIOR AO BLOCO A SER REMOVIDO
                while (blo_before->prox != i) {
                    
                    blo_before = blo_before->prox;

                }

                 if (blo_before->status == 'L') {

                    blo_before->tamanho += i->tamanho;
                    blo_before->prox = NULL;
                    free(i);
                    break;

                 }

                 if (blo_before->status == 'O') {

                    i->status = 'L';
                    break;

                 }

            }

            

        }

    }

}

int main(int argc, char *argv[]) {

    if (argc != 2) {

        printf("Formato\n\t\t%s <valor1>\n", argv[0]);
        printf("\t%s 0 -> Para usar FIRST-FIT\n", argv[0]);
        printf("\t%s 1 -> Para usar BEST-FIT\n", argv[0]);
		exit(1);

    }

    // 0 - Para First Fit, 1 - Best Fit
    int algoritmo = atoi(argv[1]);

    if (algoritmo != 0 && algoritmo != 1) {

        printf("\nALGORITMO INVALIDO!\n");
        exit(1);

    }

    if (algoritmo == 0) {

        printf("ALGORITMO EM USO: FIRST-FIT\n");

    } else {

        printf("ALGORITMO EM USO: BEST-FIT\n");

    }


    struct Bloco *memoria = inicializaMemoria(TAM_MEM);
    generateRandomAllocation(memoria);

    imprimirLista(memoria);

    while (1) {

        struct Bloco *troca = adicionarRemoverProcesso(memoria, algoritmo);
        memoria = troca;
        imprimirLista(memoria);

    }
    
    return 0;

}
