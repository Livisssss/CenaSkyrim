#include "Scene1.h"

CScene1::CScene1()
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

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(-620.0f, 500.0f, -900.0f, 50.0f);

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
	pTextures->CreateTextureAnisotropic(7, "../Objetos/casa/CasaTex.jpg");
	pTextures->CreateTextureAnisotropic(10, "../Objetos/casa/janela.jpg");

	// Cria um terreno a partir de um HeigthMap
	pTerreno = NULL;
	pTerreno = new CTerreno();
	pTerreno->LoadRawFile("../Scene1/T2.raw");

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

	// Carrega casa
	pCasa = NULL;
	pCasa = new CModel_3DS();
	pCasa->Load("../Objetos/casa/casa.3ds");


	// Configurando a POINT LIGHT
	fPointLightPosX = 900.0f;
	fPointLightPosY = 3022.0f;
	fPointLightPosZ = -1800.0f;
	PointLightAmbient[0] = 0.2f;	PointLightAmbient[1] = 0.2f;	PointLightAmbient[2] = 0.2f;	PointLightAmbient[3] = 1.0f;
	PointLightDiffuse[0] = 2.0f;	PointLightDiffuse[1] = 2.0f;	PointLightDiffuse[2] = 2.0f;	PointLightDiffuse[3] = 2.0f;
	PointLightSpecular[0] = 2.0f;	PointLightSpecular[1] = 2.0f;	PointLightSpecular[2] = 2.0f;	PointLightSpecular[3] = 2.0f;
	PointLightPosition[0] = fPointLightPosX;
	PointLightPosition[1] = fPointLightPosY;
	PointLightPosition[2] = fPointLightPosZ;
	PointLightPosition[3] = 1.0f;

	fLightSpeed = 10.0f;
}


CScene1::~CScene1(void)
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

	if (pTreeOld)
	{
		delete pTreeOld;
		pTreeOld = NULL;
	}

	if (pCasa)
	{
		delete pCasa;
		pCasa = NULL;
	}
}




int CScene1::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 100000)	// When A Second Has Passed...
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


	// Seta as posições da câmera
	pCamera->setView();

	// Desenha grid 
	Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do sistema cartesiano
	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Configura os parâmetros para a point light
	glLightfv(GL_LIGHT0, GL_AMBIENT, PointLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, PointLightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, PointLightSpecular);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);


	PointLightPosition[0] = fPointLightPosX;
	PointLightPosition[1] = fPointLightPosY;
	PointLightPosition[2] = fPointLightPosZ;
	PointLightPosition[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, PointLightPosition);

	// Desenha a lâmpada
	glPushMatrix();
	glTranslatef(fPointLightPosX, fPointLightPosY, fPointLightPosZ);
	glutSolidSphere(100.5, 100, 100);
	glPopMatrix();

	// Liga a point light
	glEnable(GL_LIGHT0);

	// Habilita iluminação
	glEnable(GL_LIGHTING);











	// Habilita Mapeamento de texturas
	glEnable(GL_TEXTURE_2D);


	// Desenha o SkyBox
	MatAmbient[0] = 0.1f; MatAmbient[1] = 0.1f; MatAmbient[2] = 0.3f; MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.2f; MatDiffuse[1] = 0.2f; MatDiffuse[2] = 0.4f; MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.5f; MatSpecular[1] = 0.5f; MatSpecular[2] = 0.6f; MatSpecular[3] = 1.0f;
	MatShininess = 50.0f;


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	CreateSkyBox(0.0f, 0.0f, 0.0f,  // Centraliza a SkyBox na origem
		9000.0f, 9000.0f, 9000.0f, // Dimensões
		pTextures);



	// Configuração das variáveis para FOG
	bFog = true;
	fFogDensity = 0.1f;
	iFogMode = 0;
	vFogColor[0] = 0.9f;       // Cor branca (R)
	vFogColor[1] = 0.9f;       // Cor branca (G)
	vFogColor[2] = 1.0f;       // Cor branca (B)
	vFogColor[3] = 1.0f;       // Opacidade total (A)



	if (bFog) {
		glEnable(GL_FOG); // Habilita Neblina (FOG)
		glHint(GL_FOG_HINT, GL_NICEST); // Qualidade da FOG
		glFogfv(GL_FOG_COLOR, vFogColor); // Cor da FOG
		glFogf(GL_FOG_START, 2500.0f); // Começa longe da câmera
		glFogf(GL_FOG_END, 5000.0f);  // Termina mais longe

		switch (iFogMode) {
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
	else {
		glDisable(GL_FOG); // Desativa Neblina (FOG)
	}




	// Desenha o terreno
	MatAmbient[0] = 0.4f; MatAmbient[1] = 0.4f; MatAmbient[2] = 0.6f; MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.5f; MatDiffuse[1] = 0.5f; MatDiffuse[2] = 0.6f; MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.8f; MatSpecular[1] = 0.8f; MatSpecular[2] = 0.9f; MatSpecular[3] = 1.0f;
	MatShininess = 60.0f;


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	glPushMatrix();
	glTranslatef(-3000.0f, -70.0f, -3000.0f); // Centraliza o terreno
	glScalef(4.0f, 4.0f, 4.0f);              // Escala o terreno

	pTextures->ApplyTexture(6);               // Textura de neve
	pTerreno->RenderHeightMapVA();            // Renderiza o terreno
	glPopMatrix();






	//// Desenha Casa
	//glPushMatrix();
	//glTranslatef(0.0f, 0.0f, 0.0f);
	//pCasa->Draw();
	//glPopMatrix();



	////// Casa  (VERTEX ARRAYS)
	//pTextures->ApplyTexture(7);
	//glPushMatrix();
	//glTranslatef(0.0f, 51.0f, 0.0f);
	//glScalef(3.0f, 3.0f, 3.0f);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glNormalPointer(GL_FLOAT, 0, &this->vfCasaNormals[0]);
	//glTexCoordPointer(2, GL_FLOAT, 0, &this->vfCasaTexCoords[0]);
	//glVertexPointer(3, GL_FLOAT, 0, &this->vfCasaVerts[0]);
	//glDrawArrays(GL_TRIANGLES, 0, 204);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glPopMatrix();



	//// Desenha os vidros das janelas aplicando Blending.
	//glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
	//pTextures->ApplyTexture(10);
	//glPushMatrix();
	//glTranslatef(3.0f, 55.0f, 16.0f);
	//glBegin(GL_QUADS);
	//glNormal3f(0.0f, 0.0f, 1.0f);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0, 0.0, 0.0);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0, 0.0, 0.0);
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0, 10.0, 0.0);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0, 10.0, 0.0);
	//glEnd();
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(18.0f, 55.0f, 16.0f);
	//glBegin(GL_QUADS);
	//glNormal3f(0.0f, 0.0f, 1.0f);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0, 0.0, 0.0);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0, 0.0, 0.0);
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0, 10.0, 0.0);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0, 10.0, 0.0);
	//glEnd();
	//glPopMatrix();

	//glDisable(GL_BLEND);










	// Arvore
	MatAmbient[0] = 0.1f; MatAmbient[1] = 0.05f; MatAmbient[2] = 0.02f; MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.35f; MatDiffuse[1] = 0.18f; MatDiffuse[2] = 0.12f; MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.1f; MatSpecular[1] = 0.05f; MatSpecular[2] = 0.02f; MatSpecular[3] = 1.0f;
	MatShininess = 20.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	glPushMatrix();
	glTranslatef(-1500.0f, 140.0f, -900.0f); // Define posição da árvore
	glScalef(1.0f, 1.5f, 1.5f); // Reduz a escala da árvore
	pTreeOld->Draw(); // Desenha a árvore
	glPopMatrix();

	// Pedra1
	MatAmbient[0] = 0.3f; MatAmbient[1] = 0.3f; MatAmbient[2] = 0.4f; MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.7f; MatDiffuse[1] = 0.7f; MatDiffuse[2] = 0.8f; MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.4f; MatSpecular[1] = 0.4f; MatSpecular[2] = 0.5f; MatSpecular[3] = 1.0f;
	MatShininess = 50.0f;  // Maior para mais brilho


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	glPushMatrix();
	glTranslatef(200.0f, 132.0f, -1700.0f); // Define posição da árvore
	glScalef(8.0f, 8.0f, 8.0f); // Reduz a escala da árvore
	pPedra->Draw(); // Desenha a árvore
	glPopMatrix();

	// Pedra2
	MatAmbient[0] = 0.3f; MatAmbient[1] = 0.3f; MatAmbient[2] = 0.4f; MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.7f; MatDiffuse[1] = 0.7f; MatDiffuse[2] = 0.8f; MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.4f; MatSpecular[1] = 0.4f; MatSpecular[2] = 0.5f; MatSpecular[3] = 1.0f;
	MatShininess = 50.0f;  // Maior para mais brilho

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	glPushMatrix();
	glTranslatef(-850.0f, 132.0f, 1300.0f); // Define posição da árvore
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glScalef(32.0f, 32.0f, 32.0f); // Reduz a escala da árvore
	pPedra->Draw(); // Desenha a árvore
	glPopMatrix();


	// Carroca
	MatAmbient[0] = 0.1f; MatAmbient[1] = 0.05f; MatAmbient[2] = 0.02f; MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.35f; MatDiffuse[1] = 0.18f; MatDiffuse[2] = 0.12f; MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.1f; MatSpecular[1] = 0.05f; MatSpecular[2] = 0.02f; MatSpecular[3] = 1.0f;
	MatShininess = 20.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	glPushMatrix();
	glTranslatef(-550.0f, 122.0f, -800.0f); // Define posição da árvore
	glRotatef(70.0f, 0.0f, 1.0f, 0.0f);
	glScalef(200.0f, 200.0f, 200.0f); // Reduz a escala da árvore
	pCarroca->Draw(); // Desenha a árvore
	glPopMatrix();

	// Cavalo
	MatAmbient[0] = 0.6f; MatAmbient[1] = 0.6f; MatAmbient[2] = 0.6f; MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 1.0f; MatDiffuse[1] = 1.0f; MatDiffuse[2] = 1.0f; MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 1.0f; MatSpecular[1] = 1.0f; MatSpecular[2] = 1.0f; MatSpecular[3] = 1.0f;
	MatShininess = 70.0f;  // Aumentado para brilho mais forte


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);

	glPushMatrix();
	glTranslatef(-350.0f, 142.0f, -1400.0f); // Define posição da árvore
	glRotatef(155.0f, 0.0f, 1.0f, 0.0f);
	glScalef(180.0f, 180.0f, 180.0f); // Reduz a escala da árvore
	pCavalo->Draw(); // Desenha a árvore
	glPopMatrix();









	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
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

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime() / 1000));

	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;

}




void CScene1::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene1::KeyPressed(void) // Tratamento de teclas pressionadas
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
	// Senão, interrompe movimento do Player
	else
	{
	}

	if (GetKeyState(VK_UP) & 0x80)
	{
		fPointLightPosZ -= fLightSpeed;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		fPointLightPosZ += fLightSpeed;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		fPointLightPosX -= fLightSpeed;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		fPointLightPosX += fLightSpeed;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		fPointLightPosY += fLightSpeed;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		fPointLightPosY -= fLightSpeed;
	}

}

void CScene1::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
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
void CScene1::Draw3DSGrid(float width, float length)
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



void CScene1::DrawAxis()
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


void CScene1::CreateSkyBox(float x, float y, float z,
	float width, float height, float length,
	CTexture* pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posição especificada(x, y, z)
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