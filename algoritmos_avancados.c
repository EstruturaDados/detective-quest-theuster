#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------
// ESTRUTURAS DE DADOS
// -----------------------------

// Estrutura da sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura da árvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// Estrutura da tabela hash
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

// -----------------------------
// CONSTANTES
// -----------------------------
#define TAM_HASH 10

// -----------------------------
// FUNÇÕES DE ÁRVORE BINÁRIA (MAPA)
// -----------------------------

// Cria uma nova sala com nome e pista
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// -----------------------------
// FUNÇÕES DE ÁRVORE DE PISTAS (BST)
// -----------------------------

// Insere pista em ordem alfabética
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* nova = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(nova->pista, pista);
        nova->esq = nova->dir = NULL;
        return nova;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

// Exibe as pistas em ordem alfabética
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf(" - %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

// -----------------------------
// FUNÇÕES DE TABELA HASH
// -----------------------------

int hashFunc(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

void inserirNaHash(HashNode* tabela[], const char* pista, const char* suspeito) {
    int indice = hashFunc(pista);
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

char* encontrarSuspeito(HashNode* tabela[], const char* pista) {
    int indice = hashFunc(pista);
    HashNode* atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Conta quantas pistas apontam para determinado suspeito
int contarPistasSuspeito(HashNode* tabela[], const char* suspeito) {
    int cont = 0;
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabela[i];
        while (atual != NULL) {
            if (strcmp(atual->suspeito, suspeito) == 0)
                cont++;
            atual = atual->prox;
        }
    }
    return cont;
}

// -----------------------------
// FUNÇÃO DE EXPLORAÇÃO
// -----------------------------

void explorarSalas(Sala* atual, PistaNode** arvorePistas, HashNode* tabela[]) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        printf("Escolha o caminho (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &opcao);

        if (opcao == 'e')
            atual = atual->esquerda;
        else if (opcao == 'd')
            atual = atual->direita;
        else if (opcao == 's')
            break;
        else
            printf("Opção inválida!\n");
    }
}

// -----------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------

int main() {
    // Criação da mansão (árvore binária fixa)
    Sala* hall = criarSala("Hall de Entrada", "Pegada de lama");
    hall->esquerda = criarSala("Cozinha", "Copo quebrado");
    hall->direita = criarSala("Sala de Estar", "Lenço perfumado");

    hall->esquerda->esquerda = criarSala("Despensa", "");
    hall->esquerda->direita = criarSala("Jardim", "Pegada de sapato");
    hall->direita->esquerda = criarSala("Biblioteca", "Carta rasgada");
    hall->direita->direita = criarSala("Quarto", "Perfume caro");

    // Árvore de pistas e tabela hash
    PistaNode* arvorePistas = NULL;
    HashNode* tabela[TAM_HASH];
    for (int i = 0; i < TAM_HASH; i++)
        tabela[i] = NULL;

    // Relações pista → suspeito
    inserirNaHash(tabela, "Pegada de lama", "Jardineiro");
    inserirNaHash(tabela, "Pegada de sapato", "Jardineiro");
    inserirNaHash(tabela, "Lenço perfumado", "Madame Clarisse");
    inserirNaHash(tabela, "Perfume caro", "Madame Clarisse");
    inserirNaHash(tabela, "Copo quebrado", "Mordomo");
    inserirNaHash(tabela, "Carta rasgada", "Professor Edgar");

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão e colete pistas...\n");

    explorarSalas(hall, &arvorePistas, tabela);

    printf("\n===== PISTAS COLETADAS =====\n");
    exibirPistas(arvorePistas);

    char suspeito[50];
    printf("\nQuem você acha que é o culpado? ");
    scanf(" %[^\n]", suspeito);

    int qtd = contarPistasSuspeito(tabela, suspeito);
    if (qtd >= 2)
        printf("\n Você acertou! As pistas realmente apontam para %s!\n", suspeito);
    else
        printf("\n Não há provas suficientes contra %s.\n", suspeito);

    printf("\nFim do jogo. Obrigado por jogar Detective Quest!\n");

    return 0;
}
