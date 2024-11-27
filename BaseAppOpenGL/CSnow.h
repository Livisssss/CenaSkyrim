#pragma once

#include <vector>
#include "CTexture.h"

struct Snowflake
{
    float x, y, z; // Posição da floco de neve
    float speed;   // Velocidade de queda
    float size;    // Tamanho do floco
};

class CSnow
{
public:
    CSnow(int maxFlakes, float areaWidth, float areaHeight, float areaDepth);
    ~CSnow();

    void Initialize(); // Inicializa as posições e velocidades dos flocos
    void Update(float deltaTime); // Atualiza a posição dos flocos
    void Render(); // Renderiza os flocos de neve

    void SetTexture(CTexture* texture); // Define a textura dos flocos

private:
    int maxFlakes;                // Número máximo de flocos
    float areaWidth, areaHeight, areaDepth; // Dimensões da área da neve
    std::vector<Snowflake> flakes; // Lista de flocos
    CTexture* pTexture;           // Textura do floco
};
