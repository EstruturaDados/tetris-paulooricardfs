/*
  TETRIS STACK — Fila e Pilha de Peças
  Desenvolvido por: Paulo Ferreira (ByteBros Challenge)
  Descrição:
    Simula a gestão de peças do jogo Tetris Stack,
    com uma fila circular de peças futuras e uma
    pilha de peças reservadas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

/* --- Estrutura que representa uma peça --- */
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

/* --- Estrutura da fila circular --- */
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

/* --- Estrutura da pilha --- */
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

/* --- Protótipos das funções --- */
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);

void exibirFila(Fila *f);
void exibirPilha(Pilha *p);

void enqueue(Fila *f, Peca nova);
Peca dequeue(Fila *f);
void push(Pilha *p, Peca nova);
Peca pop(Pilha *p);

Peca gerarPeca(void);

/* --- Implementação principal --- */
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n===== ESTADO ATUAL =====\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nAções disponíveis:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida!\n");
            continue;
        }

        if (opcao == 1) {
            if (filaVazia(&fila)) {
                printf("\nFila vazia! Nenhuma peça para jogar.\n");
            } else {
                Peca jogada = dequeue(&fila);
                printf("\nVocê jogou a peça [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca()); // gera nova peça
            }
        } 
        else if (opcao == 2) {
            if (filaVazia(&fila)) {
                printf("\nNenhuma peça disponível na fila!\n");
            } else if (pilhaCheia(&pilha)) {
                printf("\nPilha cheia! Não é possível reservar mais peças.\n");
            } else {
                Peca reservada = dequeue(&fila);
                push(&pilha, reservada);
                printf("\nPeça [%c %d] reservada com sucesso!\n", reservada.nome, reservada.id);
                enqueue(&fila, gerarPeca()); // nova peça entra na fila
            }
        } 
        else if (opcao == 3) {
            if (pilhaVazia(&pilha)) {
                printf("\nNenhuma peça reservada para usar.\n");
            } else {
                Peca usada = pop(&pilha);
                printf("\nVocê usou a peça [%c %d] da reserva.\n", usada.nome, usada.id);
            }
        } 
        else if (opcao == 0) {
            printf("\nEncerrando o jogo... Até a próxima!\n");
        } 
        else {
            printf("\nOpção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

/* --- Inicializa estruturas --- */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

/* --- Funções de verificação --- */
int filaVazia(Fila *f) { return (f->quantidade == 0); }
int filaCheia(Fila *f) { return (f->quantidade == TAM_FILA); }
int pilhaVazia(Pilha *p) { return (p->topo == -1); }
int pilhaCheia(Pilha *p) { return (p->topo == TAM_PILHA - 1); }

/* --- Funções de exibição --- */
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");
    if (filaVazia(f)) {
        printf("[Vazia]\n");
        return;
    }
    for (int i = 0; i < f->quantidade; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base):\n");
    if (pilhaVazia(p)) {
        printf("[Vazia]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

/* --- Operações da fila --- */
void enqueue(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = nova;
    f->quantidade++;
}

Peca dequeue(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

/* --- Operações da pilha --- */
void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = nova;
}

Peca pop(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) return removida;
    removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

/* --- Gera uma nova peça aleatória --- */
Peca gerarPeca(void) {
    static int contadorID = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}
