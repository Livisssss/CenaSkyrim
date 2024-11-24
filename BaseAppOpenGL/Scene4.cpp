#include "Scene4.h"

CScene4::CScene4()
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
	pCamera = new CCamera(0.0f, 1.0f, 20.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();
	pTextures->CreateTextureAnisotropic(1, "../Scene1/churchwall2a.bmp");
	pTextures->CreateTextureAnisotropic(2, "../Scene1/TriangleFaces.bmp");


	// Configurando a POINT LIGHT
	fPointLightPosX = -10.0f;
	fPointLightPosY = 5.0f;
	fPointLightPosZ = 10.0f;
	PointLightAmbient[0] = 1.0f;	PointLightAmbient[1] = 1.0f;	PointLightAmbient[2] = 1.0f;	PointLightAmbient[3] = 1.0f;
	PointLightDiffuse[0] = 1.0f;	PointLightDiffuse[1] = 1.0f;	PointLightDiffuse[2] = 1.0f;	PointLightDiffuse[3] = 1.0f;
	PointLightSpecular[0] = 1.0f;	PointLightSpecular[1] = 1.0f;	PointLightSpecular[2] = 1.0f;	PointLightSpecular[3] = 1.0f;
	PointLightPosition[0] = fPointLightPosX;
	PointLightPosition[1] = fPointLightPosY;
	PointLightPosition[2] = fPointLightPosZ;
	PointLightPosition[3] = 1.0f;

	// Configurando a SPOT LIGHT
	fSpotLightPosX = 10.0f;
	fSpotLightPosY = 5.0f;
	fSpotLightPosZ = 10.0f;
	SpotLightAmbient[0] = 1.0f;	SpotLightAmbient[1] = 1.0f;	SpotLightAmbient[2] = 1.0f;	SpotLightAmbient[3] = 1.0f;
	SpotLightDiffuse[0] = 1.0f;	SpotLightDiffuse[1] = 1.0f;	SpotLightDiffuse[2] = 1.0f;	SpotLightDiffuse[3] = 1.0f;
	SpotLightSpecular[0] = 1.0f;	SpotLightSpecular[1] = 1.0f;	SpotLightSpecular[2] = 1.0f;	SpotLightSpecular[3] = 1.0f;
	SpotLightDirection[0] = 0.0f;	SpotLightDirection[1] = -1.0f;	SpotLightDirection[2] = 0.0f;	SpotLightDirection[3] = 1.0f;
	fSpotAttenuation = 10.0f;
	fSpotCutOff = 45.0f;
	SpotLightPosition[0] = fSpotLightPosX;
	SpotLightPosition[1] = fSpotLightPosY;
	SpotLightPosition[2] = fSpotLightPosZ;
	SpotLightPosition[3] = 1.0f;

	bIsSpotActive = false;
	fLightSpeed = 0.2f;
}


CScene4::~CScene4(void)
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
}




int CScene4::DrawGLScene(void)	// Função que desenha a cena
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();



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



	if (bIsSpotActive)
	{
		// Configura os parâmetros para a fonte de luz (Emissora)
		glLightfv(GL_LIGHT1, GL_AMBIENT, SpotLightAmbient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, SpotLightDiffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, SpotLightSpecular);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, SpotLightDirection);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, fSpotAttenuation);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, fSpotCutOff);

		SpotLightPosition[0] = fSpotLightPosX;
		SpotLightPosition[1] = fSpotLightPosY;
		SpotLightPosition[2] = fSpotLightPosZ;
		SpotLightPosition[3] = 1.0f;
		glLightfv(GL_LIGHT1, GL_POSITION, SpotLightPosition);

		// Desenha a lâmpada
		glPushMatrix();
		glTranslatef(fSpotLightPosX, fSpotLightPosY, fSpotLightPosZ);
		glutSolidSphere(0.5, 10, 10);
		glPopMatrix();

		glEnable(GL_LIGHT1);	// Liga a Luz 0
	}
	else
	{
		// Configura os parâmetros para a fonte de luz (Emissora)
		glLightfv(GL_LIGHT0, GL_AMBIENT, PointLightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, PointLightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, PointLightSpecular);

		PointLightPosition[0] = fPointLightPosX;
		PointLightPosition[1] = fPointLightPosY;
		PointLightPosition[2] = fPointLightPosZ;
		PointLightPosition[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, PointLightPosition);

		// Desenha a lâmpada
		glPushMatrix();
		glTranslatef(fPointLightPosX, fPointLightPosY, fPointLightPosZ);
		glutSolidSphere(0.5, 10, 10);
		glPopMatrix();

		glEnable(GL_LIGHT0);	// Liga a Luz 0
	}

	glEnable(GL_LIGHTING); // Habilita iluminação
	


	// Cria um cubo
	// Configura os parâmetros para o material do objeto (Reflexão)
	// Define propriedades do material do objeto
	MatAmbient[0] = 0.1745f;	MatAmbient[1] = 0.01175f;	MatAmbient[2] = 0.01175f;	MatAmbient[3] = 0.55f;
	MatDiffuse[0] = 0.61424f;	MatDiffuse[1] = 0.04136f;	MatDiffuse[2] = 0.04136f;	MatDiffuse[3] = 0.55f;
	MatSpecular[0] = 0.727811f;	MatSpecular[1] = 0.626959f;	MatSpecular[2] = 0.626959f;	MatSpecular[3] = 0.55f;
	MatShininess = 76.8f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
	glPushMatrix();
	glTranslatef(10.0f, 0.0f, 0.0f);
	glutSolidCube(3.0);
	glPopMatrix();


	// Cria um teapot
	MatAmbient[0] = 0.2295f;	MatAmbient[1] = 0.08825f;	MatAmbient[2] = 0.0275f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.5508f;	MatDiffuse[1] = 0.2118f;	MatDiffuse[2] = 0.066f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.580594f;	MatSpecular[1] = 0.223257f;	MatSpecular[2] = 0.0695701f;	MatSpecular[3] = 1.0f;
	MatShininess = 51.2f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glutSolidTeapot(3.0);
	glPopMatrix();


	// Desenha o chão
	MatAmbient[0] = 0.0f;	MatAmbient[1] = 0.1f;	MatAmbient[2] = 0.0f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.0f;	MatDiffuse[1] = 1.0f;	MatDiffuse[2] = 0.0f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 1.0f;	MatSpecular[1] = 1.0f;	MatSpecular[2] = 1.0f;	MatSpecular[3] = 1.0f;
	MatShininess = 128.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
	glPushMatrix();
	glTranslatef(0.0f, -3.0f, 0.0f);
	Draw3DSGrid(20.0f, 20.0f);
	glPopMatrix();



	glEnable(GL_TEXTURE_2D);
	MatAmbient[0] = 0.1f;	MatAmbient[1] = 0.1f;	MatAmbient[2] = 0.1f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 1.0f;	MatDiffuse[1] = 1.0f;	MatDiffuse[2] = 1.0f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 1.0f;	MatSpecular[1] = 1.0f;	MatSpecular[2] = 1.0f;	MatSpecular[3] = 1.0f;
	MatShininess = 128.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
	DrawPyramid(-10.0f, 0.0f, 0.0f);
	glDisable(GL_TEXTURE_2D);


	if (bIsSpotActive)
		glDisable(GL_LIGHT1);	// Desliga a Luz 1
	else
		glDisable(GL_LIGHT0);	// Desliga a Luz 0

	glDisable(GL_LIGHTING); // Desabilita iluminação


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
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

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




void CScene4::MouseMove(void) // Tratamento de movimento do mouse
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

void CScene4::KeyPressed(void) // Tratamento de teclas pressionadas
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
		if (bIsSpotActive)
			fSpotLightPosZ -= fLightSpeed;
		else
			fPointLightPosZ -= fLightSpeed;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosZ += fLightSpeed;
		else
			fPointLightPosZ += fLightSpeed;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosX -= fLightSpeed;
		else
			fPointLightPosX -= fLightSpeed;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosX += fLightSpeed;
		else
			fPointLightPosX += fLightSpeed;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosY += fLightSpeed;
		else
			fPointLightPosY += fLightSpeed;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosY -= fLightSpeed;
		else 
			fPointLightPosY -= fLightSpeed;
	}

}

void CScene4::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
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
		bIsSpotActive = !bIsSpotActive;
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene4::Draw3DSGrid(float width, float length)
{

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glColor3f(0.0f, 0.3f, 0.0f);
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



void CScene4::DrawAxis()
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


void CScene4::DrawPyramid(float x, float y, float z)
{
	// Desenhar a Pirâmide
	glPushMatrix();
	glTranslatef(x, y, z);
	pTextures->ApplyTexture(2);
	glBegin(GL_TRIANGLES);

	// face frente
	glm::vec3 N, v1, v2, v3;
	v1 = glm::vec3(-2.0f, -2.0f, 2.0f);
	v2 = glm::vec3(2.0f, -2.0f, 2.0f);
	v3 = glm::vec3(0.0f, 4.0f, 0.0f);

	N = CalculateTriangleNormalVector(v1, v2, v3);
	glNormal3f(N.x, N.y, N.z);
	glTexCoord2d(0.02f, 0.1f); glVertex3f(-2.0f, -2.0f, 2.0f);
	glTexCoord2d(0.435f, 0.1f); glVertex3f(2.0f, -2.0f, 2.0f);
	glTexCoord2d(0.23f, 0.45f); glVertex3f(0.0f, 4.0f, 0.0f);

	// face trás
	v1 = glm::vec3(2.0f, -2.0f, -2.0f);
	v2 = glm::vec3(-2.0f, -2.0f, -2.0f);
	v3 = glm::vec3(0.0f, 4.0f, 0.0f);

	N = CalculateTriangleNormalVector(v1, v2, v3);
	glNormal3f(N.x, N.y, N.z);
	glTexCoord2d(0.52f, 0.1f); glVertex3f(2.0f, -2.0f, -2.0f);
	glTexCoord2d(0.94f, 0.1f); glVertex3f(-2.0f, -2.0f, -2.0f);
	glTexCoord2d(0.73f, 0.45f); glVertex3f(0.0f, 4.0f, 0.0f);

	// face direita
	v1 = glm::vec3(2.0f, -2.0f, 2.0f);
	v2 = glm::vec3(2.0f, -2.0f, -2.0f);
	v3 = glm::vec3(0.0f, 4.0f, 0.0f);

	N = CalculateTriangleNormalVector(v1, v2, v3);
	glNormal3f(N.x, N.y, N.z);
	glTexCoord2d(0.02f, 0.56f); glVertex3f(2.0f, -2.0f, 2.0f);
	glTexCoord2d(0.435f, 0.56f); glVertex3f(2.0f, -2.0f, -2.0f);
	glTexCoord2d(0.23f, 0.9f); glVertex3f(0.0f, 4.0f, 0.0f);

	// face esquerda
	v1 = glm::vec3(-2.0f, -2.0f, -2.0f);
	v2 = glm::vec3(-2.0f, -2.0f, 2.0f);
	v3 = glm::vec3(0.0f, 4.0f, 0.0f);

	N = CalculateTriangleNormalVector(v1, v2, v3);
	glNormal3f(N.x, N.y, N.z);
	glTexCoord2d(0.53f, 0.56f); glVertex3f(-2.0f, -2.0f, -2.0f);
	glTexCoord2d(0.93f, 0.56f); glVertex3f(-2.0f, -2.0f, 2.0f);
	glTexCoord2d(0.73f, 0.9f); glVertex3f(0.0f, 4.0f, 0.0f);

	glEnd();
	glPopMatrix();
}


glm::vec3 CScene4::CalculateTriangleNormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	// Calcula os dois vetores no mesmo plano utilizando
	// os vértices do triangulo
	glm::vec3 A, B;
	A.x = v2.x - v1.x;
	A.y = v2.y - v1.y;
	A.z = v2.z - v1.z;

	B.x = v3.x - v1.x;
	B.y = v3.y - v1.y;
	B.z = v3.z - v1.z;

	// Calcula o Cross Product (Encontra o vetor perpendicular
	// aos vetores A e B), ou seja, o VETOR NORMAL
	glm::vec3 NormalVector;
	NormalVector.x = A.y * B.z - A.z * B.y;
	NormalVector.y = A.z * B.x - A.x * B.z;
	NormalVector.z = A.x * B.y - A.y * B.x;

	// Calcula a magnitude do vetor normal
	double magnitude = sqrt(
		NormalVector.x * NormalVector.x +
		NormalVector.y * NormalVector.y +
		NormalVector.z * NormalVector.z);

	// Normalizar o vetor normal
	glm::vec3 NormalizedNormalVector;
	NormalizedNormalVector.x = NormalVector.x / magnitude;
	NormalizedNormalVector.y = NormalVector.y / magnitude;
	NormalizedNormalVector.z = NormalVector.z / magnitude;

	return NormalizedNormalVector;
}