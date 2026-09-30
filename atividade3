#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define RAIO_JOGADOR   20.0f
#define TOTAL_ITENS    15

typedef struct {
    float dano;
    int alcance;
} DadosArma;

typedef struct {
    int cura;
} DadosPocao;

typedef struct {
    int absorcao;
} DadosEscudo;

typedef union {
    DadosArma   arma;
    DadosPocao  pocao;
    DadosEscudo escudo;
} DadosItem;

typedef enum {
    ITEM_ARMA,
    ITEM_POCAO,
    ITEM_ESCUDO
} TipoItem;

typedef struct {
    Vector2   pos;
    float     raio;
    TipoItem  tipo;
    DadosItem dados;
    bool      coletado;
    float     tempoColeta;
} Item;

typedef struct {
    Vector2 pos;
    float   raio;
    int     vida;
    float   dano;
    int     armadura;
} Jogador;

Item *criarItens(int quantidade) {
    Item *itens = (Item *)malloc(quantidade * sizeof(Item));
    if (itens == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        Item *it = (itens + i);

        it->pos = (Vector2){
            (float)GetRandomValue(30, LARGURA_JANELA - 30),
            (float)GetRandomValue(30, ALTURA_JANELA - 30)
        };
        it->raio = 12.0f;
        it->coletado = false;
        it->tempoColeta = 0.0f;

        it->tipo = (TipoItem)GetRandomValue(ITEM_ARMA, ITEM_ESCUDO);

        if (it->tipo == ITEM_ARMA) {
            it->dados.arma.dano = (float)GetRandomValue(2, 8);
            it->dados.arma.alcance = GetRandomValue(1, 3);
        } else if (it->tipo == ITEM_POCAO) {
            int venenoSorteio = GetRandomValue(0, 9);
            if (venenoSorteio < 3) {
                it->dados.pocao.cura = -GetRandomValue(10, 25);
            } else {
                it->dados.pocao.cura = GetRandomValue(10, 30);
            }
        } else if (it->tipo == ITEM_ESCUDO) {
            it->dados.escudo.absorcao = GetRandomValue(5, 20);
        }
    }

    return itens;
}

void aplicarItem(Jogador *j, Item *item) {
    switch (item->tipo) {
        case ITEM_ARMA:
            j->dano += item->dados.arma.dano;
            break;

        case ITEM_POCAO:
            j->vida += item->dados.pocao.cura;
            if (j->vida < 0) {
                j->vida = 0;
            }
            break;

        case ITEM_ESCUDO:
            j->armadura += item->dados.escudo.absorcao;
            break;
    }
    item->coletado = true;
    item->tempoColeta = (float)GetTime();
}

bool tentarColetar(Item *item, Jogador *j) {
    if (item->coletado) return false;

    float dx = item->pos.x - j->pos.x;
    float dy = item->pos.y - j->pos.y;
    float distancia = (dx * dx + dy * dy);
    float somaRaios = (item->raio + j->raio) * (item->raio + j->raio);

    if (distancia <= somaRaios) {
        aplicarItem(j, item);
        return true;
    }
    return false;
}

void desenharItem(Item *m) {
    if (m->coletado) return;

    Color cor;
    switch (m->tipo) {
        case ITEM_ARMA:
            cor = RED;
            break;
        case ITEM_POCAO:
            if (m->dados.pocao.cura < 0) {
                cor = PURPLE;
            } else {
                cor = GREEN;
            }
            break;
        case ITEM_ESCUDO:
            cor = ORANGE;
            break;
        default:
            cor = GRAY;
            break;
    }

    DrawCircleV(m->pos, m->raio, cor);
}

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(
        LARGURA_JANELA,
        ALTURA_JANELA,
        "Atividade 3 - Tagged Union (Armas, Pocoes e Escudos)"
    );

    SetTargetFPS(60);

    Jogador jogador = {
        .pos = { LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f },
        .raio = RAIO_JOGADOR,
        .vida = 100,
        .dano = 10.0f,
        .armadura = 0
    };

    Item *itens = criarItens(TOTAL_ITENS);
    if (itens == NULL) {
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose()) {
        float vel = 250.0f * GetFrameTime();

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) jogador.pos.x += vel;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) jogador.pos.x -= vel;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) jogador.pos.y -= vel;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) jogador.pos.y += vel;

        for (int i = 0; i < TOTAL_ITENS; i++) {
            Item *it = (itens + i);

            tentarColetar(it, &jogador);

            if (it->coletado && (GetTime() - it->tempoColeta >= 4.0)) {
                it->pos = (Vector2){
                    (float)GetRandomValue(30, LARGURA_JANELA - 30),
                    (float)GetRandomValue(30, ALTURA_JANELA - 30)
                };
                it->tipo = (TipoItem)GetRandomValue(ITEM_ARMA, ITEM_ESCUDO);

                if (it->tipo == ITEM_ARMA) {
                    it->dados.arma.dano = (float)GetRandomValue(2, 8);
                    it->dados.arma.alcance = GetRandomValue(1, 3);
                } else if (it->tipo == ITEM_POCAO) {
                    int venenoSorteio = GetRandomValue(0, 9);
                    if (venenoSorteio < 3) {
                        it->dados.pocao.cura = -GetRandomValue(10, 25);
                    } else {
                        it->dados.pocao.cura = GetRandomValue(10, 30);
                    }
                } else if (it->tipo == ITEM_ESCUDO) {
                    it->dados.escudo.absorcao = GetRandomValue(5, 20);
                }

                it->coletado = false;
                it->tempoColeta = 0.0f;
            }
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            for (int i = 0; i < TOTAL_ITENS; i++) {
                desenharItem(itens + i);
            }

            DrawCircleV(jogador.pos, jogador.raio, BLUE);

            DrawText(TextFormat("Vida: %d", jogador.vida), 10, 10, 20, (jogador.vida > 30) ? DARKGREEN : RED);
            DrawText(TextFormat("Dano: %.1f", jogador.dano), 10, 35, 20, MAROON);
            DrawText(TextFormat("Armadura: %d", jogador.armadura), 10, 60, 20, DARKBLUE);

            DrawText("Vermelho: Arma (+Dano) | Laranja: Escudo (+Armadura)", 10, ALTURA_JANELA - 45, 15, DARKGRAY);
            DrawText("Verde: Pocao (+Vida)   | Roxo: Veneno (-Vida)", 10, ALTURA_JANELA - 25, 15, DARKGRAY);

        EndDrawing();
    }

    free(itens);

    CloseWindow();

    return 0;
}
