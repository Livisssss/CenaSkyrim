#pragma once

#include <vector>
#include "CTexture.h"

struct Snowflake
{
    float x, y, z; // Posi��o da floco de neve
    float speed;   // Velocidade de queda
    float size;    // Tamanho do floco
};

class CSnow
{
public:
    CSnow(int maxFlakes, float areaWidth, float areaHeight, float areaDepth);
    ~CSnow();

    void Initialize(); // Inicializa as posi��es e velocidades dos flocos
    void Update(float deltaTime); // Atualiza a posi��o dos flocos
    void Render(); // Renderiza os flocos de neve

    void SetTexture(CTexture* texture); // Define a textura dos flocos

private:
    int maxFlakes;                // N�mero m�ximo de flocos
    float areaWidth, areaHeight, areaDepth; // Dimens�es da �rea da neve
    std::vector<Snowflake> flakes; // Lista de flocos
    CTexture* pTexture;           // Textura do floco
};
