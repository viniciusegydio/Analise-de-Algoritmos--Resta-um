//codigo feito por Caio troaino Collino e Vinicius Vianna
#include <stdio.h>

#define N 7
#define INVALIDO -1
#define VAZIO 0
#define PINO 1
#define MAX_MOVS 31

//Direções: cima, baixo, esquerda, direita
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

typedef struct {
    int linha_origem, coluna_origem;
    int linha_meio, coluna_meio;
    int linha_destino, coluna_destino;
}Movimento;

int tabuleiro_inicial[N][N] = {
    {-1, -1,  1,  1,  1, -1, -1},
    {-1, -1,  1,  1,  1, -1, -1},
    { 1,  1,  1,  1,  1,  1,  1},
    { 1,  1,  1,  0,  1,  1,  1},
    { 1,  1,  1,  1,  1,  1,  1},
    {-1, -1,  1,  1,  1, -1, -1},
    {-1, -1,  1,  1,  1, -1, -1}
};

int tabuleiro[N][N];
Movimento solucao[MAX_MOVS];

void copiar_tabuleiro(int destino[N][N], int origem[N][N]) {
    int i, j;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            destino[i][j] = origem[i][j];
        }
    }
}

void inicializar_tabuleiro(void) {
    copiar_tabuleiro(tabuleiro, tabuleiro_inicial);
}

int dentro_do_tabuleiro(int l, int c) {
    return l >= 0 && l < N &&
           c >= 0 && c < N &&
           tabuleiro[l][c] != INVALIDO;
}

void aplicar_movimento(Movimento m) {
    tabuleiro[m.linha_origem][m.coluna_origem] = VAZIO;
    tabuleiro[m.linha_meio][m.coluna_meio] = VAZIO;
    tabuleiro[m.linha_destino][m.coluna_destino] = PINO;
}

void desfazer_movimento(Movimento m) {
    tabuleiro[m.linha_origem][m.coluna_origem] = PINO;
    tabuleiro[m.linha_meio][m.coluna_meio] = PINO;
    tabuleiro[m.linha_destino][m.coluna_destino] = VAZIO;
}

int gerar_movimentos(Movimento movimentos[]) {
    int qtd = 0;
    int l, c, d;

    for(l = 0; l < N; l++) {
        for(c = 0; c < N; c++) {
            if(tabuleiro[l][c] != PINO) {
                continue;
            }

            for(d = 0; d < 4; d++) {
                int lm = l + dr[d];
                int cm = c + dc[d];

                int ld = l + 2 * dr[d];
                int cd = c + 2 * dc[d];

                if(dentro_do_tabuleiro(lm, cm) &&
                    dentro_do_tabuleiro(ld, cd) &&
                    tabuleiro[lm][cm] == PINO &&
                    tabuleiro[ld][cd] == VAZIO) {

                    movimentos[qtd].linha_origem = l;
                    movimentos[qtd].coluna_origem = c;

                    movimentos[qtd].linha_meio = lm;
                    movimentos[qtd].coluna_meio = cm;

                    movimentos[qtd].linha_destino = ld;
                    movimentos[qtd].coluna_destino = cd;

                    qtd++;
                }
            }
        }
    }

    return qtd;
}

int resolver(int profundidade, int quantidade_pinos) {
    Movimento movimentos[128];
    int qtd, i;

    if(quantidade_pinos == 1) {
        return tabuleiro[3][3] == PINO;
    }

    qtd = gerar_movimentos(movimentos);

    for(i = 0; i < qtd; i++) {
        Movimento m = movimentos[i];

        aplicar_movimento(m);
        solucao[profundidade] = m;

        if(resolver(profundidade + 1, quantidade_pinos - 1)) {
            return 1;
        }

        desfazer_movimento(m);
    }

    return 0;
}

void imprimir_tabuleiro_formatado(FILE *arquivo) {
    int i, j;

    for(j = 0; j < N + 2; j++) {
        fprintf(arquivo, "#");
    }
    fprintf(arquivo, "\n");

    for(i = 0; i < N; i++) {
        fprintf(arquivo, "#");

        for(j = 0; j < N; j++) {
            if (tabuleiro[i][j] == INVALIDO) {
                fprintf(arquivo, "#");
            }else if (tabuleiro[i][j] == PINO) {
                fprintf(arquivo, "o");
            }else {
                fprintf(arquivo, " ");
            }
        }

        fprintf(arquivo, "#\n");
    }

    for(j = 0; j < N + 2; j++) {
        fprintf(arquivo, "#");
    }
    fprintf(arquivo, "\n\n");
}

void gerar_arquivo_saida(const char *nome_arquivo) {
    FILE *arquivo;
    int i;

    arquivo = fopen(nome_arquivo, "w");

    if(arquivo == NULL) {
        printf("Erro ao criar o arquivo de saida.\n");
        return;
    }

    inicializar_tabuleiro();

    fprintf(arquivo, "O processamento pode demorar alguns segundos...\n");
    imprimir_tabuleiro_formatado(arquivo);

    for (i = 0; i < MAX_MOVS; i++) {
        aplicar_movimento(solucao[i]);
        imprimir_tabuleiro_formatado(arquivo);
    }

    fclose(arquivo);
}

int main(void) {
    inicializar_tabuleiro();

    printf("O processamento pode demorar alguns segundos...\n");

    if(resolver(0, 32)) {
        gerar_arquivo_saida("restaum.out");

        printf("Solucao encontrada.\n");
        printf("Arquivo restaum.out gerado com sucesso.\n");
    }else {
        printf("Nao foi possivel encontrar solucao.\n");
    }

    return 0;
}