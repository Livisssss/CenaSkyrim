#include "Scene5.h"

CScene5::CScene5()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impress�o de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(-620.0f, 500.0f, -900.0f, 2.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();

	pTextures->CreateTextureClamp(0, "../cenario/front.jpg");
	pTextures->CreateTextureClamp(1, "../cenario/back.jpg");
	pTextures->CreateTextureClamp(2, "../cenario/down.jpg");
	pTextures->CreateTextureClamp(3, "../cenario/up.jpg");
	pTextures->CreateTextureClamp(4, "../cenario/left.jpg");
	pTextures->CreateTextureClamp(5, "../cenario/right.jpg");

	pTextures->CreateTextureAnisotropic(6, "../Cenario/Terreno3.jpg");






	// Cria um terreno a partir de um HeigthMap
	pTerreno = NULL;
	pTerreno = new CTerreno();
	pTerreno->LoadRawFile("../Scene5/T2.raw");

	// Carrega arvore torta
	pTreeOld = NULL;
	pTreeOld = new CModel_3DS();
	pTreeOld->Load("../Objetos/arvore_seca/arvore.3ds");


	// Carrega pedra
	pPedra = NULL;
	pPedra = new CModel_3DS();
	pPedra->Load("../Objetos/pedra/pedra.3ds");

	// Carrega carroca
	pCarroca = NULL;
	pCarroca = new CModel_3DS();
	pCarroca->Load("../Objetos/carroca/carroca.3ds");

	// Carrega cavalo
	pCavalo = NULL;
	pCavalo = new CModel_3DS();
	pCavalo->Load("../Objetos/cavalo/cavalo.3ds");






	// Configura��o das vari�veis para FOG
	bFog = false;
	fFogDensity = 0.01f;
	iFogMode = 0;
	vFogColor[0] = 0.7f;
	vFogColor[1] = 0.7f;
	vFogColor[2] = 1.0f;
	vFogColor[3] = 1.0f;

	// Carrega o modelo 3DS
	pCasa = NULL;
	pCasa = new CModel_3DS();
	pCasa->Load("../Scene1/casa.3ds");
}


CScene5::~CScene5(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pTerreno)
	{
		delete pTerreno;
		pTerreno = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pCasa)
	{
		delete pCasa;
		pCasa = NULL;
	}

	if (pTreeOld)
	{
		delete pTreeOld;
		pTreeOld = NULL;
	}
}




int CScene5::DrawGLScene(void)	// Fun��o que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter

	pTimer->Update();							// Atualiza o timer

	glClearColor(vFogColor[0], vFogColor[1], vFogColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posi��es da c�mera
	pCamera->setView();

	// Desenha grid 
	Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do sistema cartesiano
	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espa�o)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (IN�CIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);



	// Habilita Mapeamento de texturas
	glEnable(GL_TEXTURE_2D);


	// Desenha a FOG
	if (bFog == true)
	{
		glEnable(GL_FOG); // Habilita Neblina (FOG)
		glHint(GL_FOG_HINT, GL_NICEST); // Qualidade da FOG
		glFogfv(GL_FOG_COLOR, vFogColor); // Cor da FOG
		glFogf(GL_FOG_START, 10.0); // In�cio da FOG a partir da camera
		glFogf(GL_FOG_END, 500.0); // Limite dos objetos vis�veis

		switch (iFogMode)
		{
		case 0:
			glFogi(GL_FOG_MODE, GL_LINEAR); // Tipo da FOG
			break;

		case 1:
			glFogi(GL_FOG_MODE, GL_EXP); // Tipo da FOG
			glFogf(GL_FOG_DENSITY, fFogDensity);
			break;

		case 2:
			glFogi(GL_FOG_MODE, GL_EXP2); // Tipo da FOG
			glFogf(GL_FOG_DENSITY, fFogDensity);
			break;

		default:
			break;
		}

	}

	else
	{
		// Desenha o SkyBox
		CreateSkyBox(0.0f, 100.0f, 0.0f,  // Centraliza a SkyBox na posi��o 0, 0, 0
			9000.0f, 9000.0f, 9000.0f,  // Dimens�es da SkyBox em X, Y, Z
			pTextures);


		// Desenha o terreno
		glPushMatrix();
		glTranslatef(-3000.0f, -70.0f, -3000.0f); // Centraliza o terreno na mesma origem da SkyBox
		glScalef(4.0f, 4.0f, 4.0f); // Escala para cobrir a largura e o comprimento da SkyBox
		pTextures->ApplyTexture(6);
		pTerreno->RenderHeightMapVA();
		glPopMatrix();




		// Arvore
		glPushMatrix();
		glTranslatef(-1500.0f, 140.0f, -900.0f); // Define posi��o da �rvore
		glScalef(1.0f, 1.5f, 1.5f); // Reduz a escala da �rvore
		pTreeOld->Draw(); // Desenha a �rvore
		glPopMatrix();


		// Pedra1
		glPushMatrix();
		glTranslatef(200.0f, 132.0f, -1700.0f); // Define posi��o da �rvore
		glScalef(8.0f, 8.0f, 8.0f); // Reduz a escala da �rvore
		pPedra->Draw(); // Desenha a �rvore
		glPopMatrix();

		// Pedra2
		glPushMatrix();
		glTranslatef(-850.0f, 132.0f, 1300.0f); // Define posi��o da �rvore
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glScalef(32.0f, 32.0f, 32.0f); // Reduz a escala da �rvore
		pPedra->Draw(); // Desenha a �rvore
		glPopMatrix();


		// Carroca
		glPushMatrix();
		glTranslatef(-550.0f, 122.0f, -800.0f); // Define posi��o da �rvore
		glRotatef(70.0f, 0.0f, 1.0f, 0.0f);
		glScalef(200.0f, 200.0f, 200.0f); // Reduz a escala da �rvore
		pCarroca->Draw(); // Desenha a �rvore
		glPopMatrix();

		// Cavalo
		glPushMatrix();
		glTranslatef(-350.0f, 142.0f, -1400.0f); // Define posi��o da �rvore
		glRotatef(155.0f, 0.0f, 1.0f, 0.0f);
		glScalef(180.0f, 180.0f, 180.0f); // Reduz a escala da �rvore
		pCavalo->Draw(); // Desenha a �rvore
		glPopMatrix();













		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);



		if (bFog == true)
		{
			glDisable(GL_FOG);
		}







		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//                               DESENHA OS OBJETOS DA CENA (FIM)
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		fTimerPosY = pTimer->GetTime() / 1000.0f;
		fRenderPosY += 0.2f;

		// Impress�o de texto na tela...
		// Muda para modo de proje��o ortogonal 2D
		// OBS: Desabilite Texturas e Ilumina��o antes de entrar nesse modo de proje��o
		OrthoMode(0, 0, WIDTH, HEIGHT);


		glPushMatrix();
		glTranslatef(0.0f, HEIGHT - 200, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

		// Cor da fonte
		glColor3f(1.0f, 1.0f, 0.0f);


		glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
		if (!bIsWireframe) {
			pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
		}
		else {
			pTexto->glPrint("[TAB]  Modo FILL");
		}


		//// Camera LookAt
		glRasterPos2f(10.0f, 40.0f);
		pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

		//// Posi��o do Player
		glRasterPos2f(10.0f, 60.0f);
		pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

		//// Imprime o FPS da aplica��o e o Timer
		glRasterPos2f(10.0f, 80.0f);
		pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime() / 1000));

		glRasterPos2f(10.0f, 100.0f);
		if (bFog)
			pTexto->glPrint("[ENTER] FOG: TRUE - [F] FOG MODE: %d", iFogMode);
		else
			pTexto->glPrint("[ENTER] FOG: FALSE");

		if (bFog)
		{
			glRasterPos2f(10.0f, 120.0f);
			pTexto->glPrint("[+/-] FOG Density: %.3f", fFogDensity);
		}


		glPopMatrix();

		// Muda para modo de proje��o perspectiva 3D
		PerspectiveMode();

		return true;
	}
}




void CScene5::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os c�lculos de rota��o da vis�o do Player (atrav�s das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a c�mera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene5::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Sen�o, interrompe movimento do Player
	else
	{
	}



}

void CScene5::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case VK_RETURN:
		bFog = !bFog;
		break;

	case VK_ADD:
		fFogDensity += 0.001f;
		break;

	case VK_SUBTRACT:
		fFogDensity -= 0.001f;
		break;

	case 'F':
		iFogMode++;
		if (iFogMode > 2)
			iFogMode = 0;
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene5::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene5::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}


void CScene5::CreateSkyBox(float x, float y, float z,
	float width, float height, float length,
	CTexture* pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posi��o especificada(x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Aplica a textura que representa a parte da frente do skybox (BACK map)
	pTextures->ApplyTexture(0);

	// Desenha face BACK do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
	pTextures->ApplyTexture(1);

	// Desenha face FRONT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
	pTextures->ApplyTexture(2);

	// Desenha face BOTTOM do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (UP map)
	pTextures->ApplyTexture(3);

	// Desenha face TOP do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
	pTextures->ApplyTexture(4);

	// Desenha face LEFT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
	pTextures->ApplyTexture(5);

	// Desenha face RIGHT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}