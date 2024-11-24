#pragma once
#include "SceneBaseClass.h"
#include "CTexture.h"
#include "CTimer.h"
#include "CTexto.h"
#include "CCamera.h"
#include <glm/glm.hpp>



class CScene4 : public CSceneBaseClass
{
public:
	CScene4();
	~CScene4(void);


	virtual void MouseMove(void);					// Tratamento de movimento do mouse
	virtual void KeyPressed(void);					// Tratamento de teclas pressionadas
	virtual void KeyDownPressed(WPARAM	wParam);	// Tratamento de teclas pressionadas
	virtual int DrawGLScene(void);					// Função que desenha a cena

	void Draw3DSGrid(float width, float length);
	void DrawAxis();
	void DrawPyramid(float x, float y, float z);
	glm::vec3 CalculateTriangleNormalVector(
		glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

	
private:

	bool	keys[256];		// Array usado para rotinas do teclado
	bool	active;			// Window Active Flag Set To TRUE By Default
	bool	fullscreen;		// Exibir janela em modo fullscreem (TRUE) ou em janela (FALSE)


	CCamera* pCamera;	// Gerencia câmera OpenGL
	float fDeltaY;				// Rotação da câmera OpenGL no eixo Y
	float fDeltaX;				// Rotação da câmera OpenGL no eixo X

	CTexto* pTexto;	// Objeto que gerencia texto
	CTexture* pTextures;	// Objeto que gerencia texturas
	CTimer* pTimer;	// Objeto que gerencia o timer


	int		iFPS;			// FPS and FPS Counter
	int		iFrames;		// FPS and FPS Counter
	DWORD	ulLastFPS;		// FPS and FPS Counter
	char	szTitle[256];	// FPS and FPS Counter


	bool bIsWireframe;	// Modos Wireframe/Solid
	bool bIsCameraFPS;	// Ativa modo de camera First Person Shooter (true) ou Third Person Shooter (false)

	float fRenderPosY;
	float fTimerPosY;


	// Definindo as propriedades da fonte de luz
	GLfloat PointLightAmbient[4];
	GLfloat PointLightDiffuse[4];
	GLfloat PointLightSpecular[4];
	GLfloat PointLightPosition[4];

	GLfloat SpotLightAmbient[4];
	GLfloat SpotLightDiffuse[4];
	GLfloat SpotLightSpecular[4];
	GLfloat SpotLightPosition[4];
	GLfloat SpotLightDirection[4];
	GLfloat fSpotAttenuation;
	GLfloat fSpotCutOff;

	// Definindo as propriedades do material
	GLfloat MatAmbient[4];
	GLfloat MatDiffuse[4];
	GLfloat MatSpecular[4];
	GLfloat MatShininess;

	float fPointLightPosX;
	float fPointLightPosY;
	float fPointLightPosZ;

	float fSpotLightPosX;
	float fSpotLightPosY;
	float fSpotLightPosZ;

	float fLightSpeed;
	bool bIsSpotActive;
};

