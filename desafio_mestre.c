/*
  ==============================================
      TETRIS STACK — Gerenciador de Peças Avançado
  ==============================================
  Autor: Paulo Ferreira
  Descrição:
    Simula o gerenciamento de peças em um jogo estilo Tetris,
    utilizando uma fila circular e uma pilha.
    Inclui trocas diretas e múltiplas entre as estruturas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

/* ---------- Estrutura da Peça ---------- */
typedef struct {
    char nome;  // Tipo da peça (I, O, T, L)
    int id;     // Identificador único
} Peca;

/* ---------- Estrutura da Fila Circular ---------- */
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

/* ---------- Estrutura da Pilha ---------- */
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

/* ---------- Protótipos ---------- */
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void enqueue(Fila *f, Peca nova);
Peca dequeue(Fila *f);
void push(Pilha *p, Peca nova);
Peca pop(Pilha *p);
Peca gerarPeca(void);
void exibirFila(Fila *f);
void exibirPilha(Pilha *p);
void trocarTopo(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);

/* ---------- Implementação Principal ---------- */
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n===== ESTADO ATUAL =====\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\n===== MENU DE AÇÕES =====\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (filaVazia(&fila)) {
                    printf("\n⚠️  Fila vazia! Nenhuma peça para jogar.\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }
            case 2: {
                if (pilhaCheia(&pilha)) {
                    printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
                } else if (filaVazia(&fila)) {
                    printf("\n⚠️  Nenhuma peça disponível na fila!\n");
                } else {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("\n📦 Peça [%c %d] movida para a reserva.\n", reservada.nome, reservada.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }
            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("\n⚠️  Nenhuma peça reservada para usar.\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\n🧩 Peça usada: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }
            case 4: {
                trocarTopo(&fila, &pilha);
                break;
            }
            case 5: {
                trocaMultipla(&fila, &pilha);
                break;
            }
            case 0:
                printf("\n👋 Encerrando o programa...\n");
                break;
            default:
                printf("\n⚠️  Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

/* ---------- Funções auxiliares ---------- */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) { return f->quantidade == 0; }
int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

/* ---------- Operações de Fila ---------- */
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

/* ---------- Operações de Pilha ---------- */
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

/* ---------- Exibição ---------- */
void exibirFila(Fila *f) {
    printf("\nFila de peças:\t");
    if (filaVazia(f)) {
        printf("[Vazia]");
        return;
    }
    for (int i = 0; i < f->quantidade; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base):\t");
    if (pilhaVazia(p)) {
        printf("[Vazia]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

/* ---------- Geração automática de peça ---------- */
Peca gerarPeca(void) {
    static int contadorID = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

/* ---------- Troca entre topo da pilha e frente da fila ---------- */
void trocarTopo(Fila *f, Pilha *p) {
    if (pilhaVazia(p) || filaVazia(f)) {
        printf("\n⚠️  Troca impossível: falta de peças em uma das estruturas.\n");
        return;
    }

    int idxFrente = f->frente;
    Peca temp = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\n🔁 Troca realizada entre [%c %d] (fila) e [%c %d] (pilha).\n",
           f->itens[idxFrente].nome, f->itens[idxFrente].id,
           p->itens[p->topo].nome, p->itens[p->topo].id);
}

/* ---------- Troca múltipla de 3 peças ---------- */
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) {
        printf("\n⚠️  Troca múltipla impossível: faltam peças suficientes.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % TAM_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\n🔄 Troca múltipla entre as 3 primeiras peças da fila e as 3 da pilha realizada com sucesso!\n");
}
