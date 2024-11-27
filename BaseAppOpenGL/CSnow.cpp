#include "CSnow.h"
#include <cstdlib> // Para gerar números aleatórios
#include <GL/gl.h> // Para funções OpenGL

CSnow::CSnow(int maxFlakes, float areaWidth, float areaHeight, float areaDepth)
    : maxFlakes(maxFlakes), areaWidth(areaWidth), areaHeight(areaHeight), areaDepth(areaDepth), pTexture(nullptr)
{
    flakes.resize(maxFlakes);
}

CSnow::~CSnow()
{
    // Limpeza se necessário
}

void CSnow::Initialize()
{
    for (auto& flake : flakes)
    {
        flake.x = (static_cast<float>(rand()) / RAND_MAX) * areaWidth - areaWidth / 2;
        flake.y = (static_cast<float>(rand()) / RAND_MAX) * areaHeight;
        flake.z = (static_cast<float>(rand()) / RAND_MAX) * areaDepth - areaDepth / 2;
        flake.speed = 0.1f + (static_cast<float>(rand()) / RAND_MAX) * 0.5f;
        flake.size = 0.1f + (static_cast<float>(rand()) / RAND_MAX) * 0.2f;
    }
}

void CSnow::Update(float deltaTime)
{
    for (auto& flake : flakes)
    {
        flake.y -= flake.speed * deltaTime;
        if (flake.y < 0.0f) // Reseta o floco quando atinge o solo
        {
            flake.y = areaHeight;
            flake.x = (static_cast<float>(rand()) / RAND_MAX) * areaWidth - areaWidth / 2;
            flake.z = (static_cast<float>(rand()) / RAND_MAX) * areaDepth - areaDepth / 2;
        }
    }
}

void CSnow::Render()
{
    if (!pTexture)
        return;

    glEnable(GL_TEXTURE_2D);
    pTexture->Bind();

    glBegin(GL_QUADS);
    for (const auto& flake : flakes)
    {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(flake.x - flake.size, flake.y - flake.size, flake.z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(flake.x + flake.size, flake.y - flake.size, flake.z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(flake.x + flake.size, flake.y + flake.size, flake.z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(flake.x - flake.size, flake.y + flake.size, flake.z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void CSnow::SetTexture(CTexture* texture)
{
    pTexture = texture;
}
