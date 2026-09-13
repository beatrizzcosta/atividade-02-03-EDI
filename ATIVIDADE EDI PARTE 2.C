#include "raylib.h"
#include <stdlib.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600

typedef enum {
    MOEDA_BRONZE,
    MOEDA_PRATA,
    MOEDA_OURO,
    MOEDA_DIAMANTE
} TipoMoeda;

typedef struct {
    Vector2 pos;
    float raio;
    TipoMoeda tipo;
    int valor;
    int coletada;
    float tempoColeta;
} Moeda;

Color corDaMoeda(TipoMoeda tipo) {
    switch (tipo) {
        case MOEDA_BRONZE:
            return (Color){160, 90, 40, 255};

        case MOEDA_PRATA:
            return (Color){190, 190, 190, 255};

        case MOEDA_OURO:
            return GOLD;

        case MOEDA_DIAMANTE:
            return (Color){0, 255, 255, 255};

        default:
            return WHITE;
    }
}

int valorDaMoeda(TipoMoeda tipo) {
    switch (tipo) {
        case MOEDA_BRONZE:
            return 5;

        case MOEDA_PRATA:
            return 10;

        case MOEDA_OURO:
            return 25;

        case MOEDA_DIAMANTE:
            return 50;

        default:
            return 0;
    }
}

Moeda *criarMoedas(int quantidade) {

    Moeda *moedas = (Moeda *) malloc(
        quantidade * sizeof(Moeda)
    );

    for (int i = 0; i < quantidade; i++) {

        Moeda *m = (moedas + i);

        m->pos = (Vector2) {
            GetRandomValue(30, LARGURA_JANELA - 30),
            GetRandomValue(30, ALTURA_JANELA - 30)
        };

        m->raio = 10.0f;

        int sorteio = GetRandomValue(0, 9);

        if (sorteio == 0) {
            m->tipo = MOEDA_DIAMANTE;
        } else {
            m->tipo = (TipoMoeda) GetRandomValue(
                MOEDA_BRONZE,
                MOEDA_OURO
            );
        }

        m->valor = valorDaMoeda(m->tipo);
        m->coletada = 0;
        m->tempoColeta = 0;
    }

    return moedas;
}

int tentarColetar(
    Moeda *m,
    Vector2 posJogador,
    float raioJogador
) {

    if (m->coletada)
        return 0;

    float dx = m->pos.x - posJogador.x;
    float dy = m->pos.y - posJogador.y;

    float distancia = dx * dx + dy * dy;

    float somaRaios =
        (m->raio + raioJogador) *
        (m->raio + raioJogador);

    if (distancia <= somaRaios) {

        m->coletada = 1;
        m->tempoColeta = GetTime();

        return 1;
    }

    return 0;
}

void desenharMoeda(Moeda *m) {

    if (!m->coletada) {

        DrawCircleV(
            m->pos,
            m->raio,
            corDaMoeda(m->tipo)
        );
    }
}

int main(void) {

    InitWindow(
        LARGURA_JANELA,
        ALTURA_JANELA,
        "Moedas"
    );

    SetTargetFPS(60);

    int quantidade = 10;

    Moeda *moedas = criarMoedas(quantidade);

    Vector2 jogador = {
        LARGURA_JANELA / 2,
        ALTURA_JANELA / 2
    };

    float raioJogador = 15;

    int pontuacao = 0;

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_RIGHT))
            jogador.x += 4;

        if (IsKeyDown(KEY_LEFT))
            jogador.x -= 4;

        if (IsKeyDown(KEY_UP))
            jogador.y -= 4;

        if (IsKeyDown(KEY_DOWN))
            jogador.y += 4;


        for (int i = 0; i < quantidade; i++) {

            Moeda *m = &moedas[i];

            if (tentarColetar(
                m,
                jogador,
                raioJogador
            )) {

                pontuacao += m->valor;
            }

            if (
                m->coletada &&
                GetTime() - m->tempoColeta >= 3
            ) {

                m->pos = (Vector2) {
                    GetRandomValue(30, LARGURA_JANELA - 30),
                    GetRandomValue(30, ALTURA_JANELA - 30)
                };

                m->coletada = 0;
            }
        }


        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < quantidade; i++) {
            desenharMoeda(&moedas[i]);
        }

        DrawCircleV(
            jogador,
            raioJogador,
            BLUE
        );

        DrawText(
            TextFormat("Pontuacao: %d", pontuacao),
            20,
            20,
            20,
            BLACK
        );

        EndDrawing();
    }

    free(moedas);

    CloseWindow();

    return 0;
}