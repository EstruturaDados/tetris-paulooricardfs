/*
  TETRIS STACK — Simulação da Fila de Peças Futuras
  Autor: Paulo Ferreira (ByteBros Challenge)
  Descrição:
    Implementa uma fila circular de peças do jogo Tetris Stack.
    O jogador pode:
      - Jogar uma peça (dequeue)
      - Inserir nova peça (enqueue)
      - Visualizar o estado da fila
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5

// Estrutura de dados que representa uma peça
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

/* --- Protótipos das funções --- */
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void exibirFila(Fila *f);
void enqueue(Fila *f, Peca nova);
Peca dequeue(Fila *f);
Peca gerarPeca(void);

/* --- Implementação --- */
int main() {
    srand(time(NULL)); // Semente para aleatoriedade

    Fila fila;
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n===== FILA DE PEÇAS FUTURAS =====\n");
        exibirFila(&fila);

        printf("\nAções disponíveis:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // limpa buffer
            printf("Entrada inválida.\n");
            continue;
        }

        if (opcao == 1) {
            if (filaVazia(&fila)) {
                printf("\nFila vazia! Nenhuma peça para jogar.\n");
            } else {
                Peca jogada = dequeue(&fila);
                printf("\nVocê jogou a peça [%c %d]\n", jogada.nome, jogada.id);
            }
        } else if (opcao == 2) {
            if (filaCheia(&fila)) {
                printf("\nFila cheia! Não é possível adicionar mais peças.\n");
            } else {
                Peca nova = gerarPeca();
                enqueue(&fila, nova);
                printf("\nNova peça gerada: [%c %d]\n", nova.nome, nova.id);
            }
        } else if (opcao == 0) {
            printf("\nEncerrando o jogo... Até a próxima!\n");
        } else {
            printf("\nOpção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

/* --- Inicializa a fila --- */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

/* --- Verifica se a fila está vazia --- */
int filaVazia(Fila *f) {
    return (f->quantidade == 0);
}

/* --- Verifica se a fila está cheia --- */
int filaCheia(Fila *f) {
    return (f->quantidade == TAM_FILA);
}

/* --- Exibe o estado atual da fila --- */
void exibirFila(Fila *f) {
    if (filaVazia(f)) {
        printf("\n[Fila vazia]\n");
        return;
    }

    printf("\nFila de peças:\n");
    for (int i = 0; i < f->quantidade; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

/* --- Insere nova peça no final da fila (enqueue) --- */
void enqueue(Fila *f, Peca nova) {
    if (filaCheia(f)) {
        printf("\nErro: Fila cheia.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = nova;
    f->quantidade++;
}

/* --- Remove peça da frente da fila (dequeue) --- */
Peca dequeue(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) {
        printf("\nErro: Fila vazia.\n");
        return removida;
    }
    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

/* --- Gera nova peça com nome e id únicos --- */
Peca gerarPeca(void) {
    static int contadorID = 0; // mantém id único
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}
