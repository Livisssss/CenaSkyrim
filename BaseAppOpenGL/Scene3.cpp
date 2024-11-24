#include "Scene3.h"

CScene3::CScene3()
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
	//pTextures = new CTexture();	

	fAngle = 0.0f;
	fX = 15.0f;
	fY = 0.0f;
	fZ = 0.0f;
	fScale = 0.5f;

	rotCounterClockWise = true;
	rotArms = 0.0f;

	enemyPos = glm::vec3(0.0f, 0.0f, 0.0);
	playerPos = glm::vec3(0.0f, 0.0f, 0.0);
	enemyVel = 0.01f;
}


CScene3::~CScene3(void)
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




int CScene3::DrawGLScene(void)	// Função que desenha a cena
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
	fAngle += 1.0f;
	if (fAngle >= 360.0f)
		fAngle = 0.0f;


	glPushMatrix();
	glTranslatef(fX, fY, fZ);
	glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
	glScalef(fScale, fScale, fScale);

	glBegin(GL_QUADS);
	// Face da frente
	glColor4ub(255, 0, 0, 255);
	glVertex3f(-5.0f, -5.0, 5.0f);
	glVertex3f( 5.0f, -5.0, 5.0f);
	glVertex3f( 5.0f,  5.0, 5.0f);
	glVertex3f(-5.0f,  5.0, 5.0f);

	// Face de tras
	glColor4ub(255, 255, 0, 255);
	glVertex3f(-5.0f, -5.0, -5.0f);
	glVertex3f( 5.0f, -5.0, -5.0f);
	glVertex3f( 5.0f,  5.0, -5.0f);
	glVertex3f(-5.0f,  5.0, -5.0f);

	// Face da direita
	glColor4ub(0, 255, 0, 255);
	glVertex3f(5.0f, -5.0,  5.0f);
	glVertex3f(5.0f, -5.0, -5.0f);
	glVertex3f(5.0f,  5.0, -5.0f);
	glVertex3f(5.0f,  5.0,  5.0f);

	// Face da esquerda
	glColor4ub(0, 0, 255, 255);
	glVertex3f(-5.0f, -5.0, 5.0f);
	glVertex3f(-5.0f, -5.0, -5.0f);
	glVertex3f(-5.0f, 5.0, -5.0f);
	glVertex3f(-5.0f, 5.0, 5.0f);
	glEnd();


	glBegin(GL_TRIANGLES);
	// Face frente
	glColor4ub(130, 210, 199, 255);
	glVertex3f(-5.0f, 5.0f, 5.0f);
	glVertex3f( 5.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);

	// Face tras
	glColor4ub(130, 50, 50, 255);
	glVertex3f(-5.0f, 5.0f, -5.0f);
	glVertex3f(5.0f, 5.0f, -5.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);

	// Face direita
	glColor4ub(50, 150, 230, 255);
	glVertex3f(5.0f, 5.0f, -5.0f);
	glVertex3f(5.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);

	// Face esquerda
	glColor4ub(50, 210, 25, 255);
	glVertex3f(-5.0f, 5.0f, -5.0f);
	glVertex3f(-5.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glEnd();

	glPopMatrix();




	// Desenha um Robo
	CreateRobot(-10.0f, 0.0f, 0.0f);

	

	// Atualiza a posição do Player
	playerPos = glm::vec3(pCamera->Position[0], 
						  pCamera->Position[1], 
						  pCamera->Position[2]);

	// Calcula o vetor de direção do inimigo para o jogador
	// Inimigo ---> Jogador
	glm::vec3 dirVectorPE(playerPos - enemyPos);

	// Normaliza o vetor de direção
	glm::normalize(dirVectorPE);

	// Atualiza a nova posição do inimigo
	enemyPos += (dirVectorPE * enemyVel);

	// Calcula a distância entre o jogador e o inimigo
	double distance = sqrt(
		pow(enemyPos.x - playerPos.x, 2) +
		pow(enemyPos.y - playerPos.y, 2) +
		pow(enemyPos.z - playerPos.z, 2));

	// Mudar a cor do inimigo de acordo com a distância do jogador
	if (distance <= 0.5)
		glColor3ub(255, 0, 0);
	else
		glColor3ub(255, 255, 255);

	// Desenha o inimigo
	glPushMatrix();
	glTranslatef(enemyPos.x, enemyPos.y, enemyPos.z);
	auxSolidTeapot(0.2);
	glPopMatrix();










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
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));

	// Imprime a distancia
	glRasterPos2f(10.0f, 100.0f);
	pTexto->glPrint("Distance: %f", distance);

	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene3::MouseMove(void) // Tratamento de movimento do mouse
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

void CScene3::KeyPressed(void) // Tratamento de teclas pressionadas
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
		fZ -= 1.0f;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		fZ += 1.0f;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		fX += 1.0f;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		fX -= 1.0f;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		fY += 1.0f;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		fY -= 1.0f;
	}

	if (GetKeyState(VK_ADD) & 0x80)
	{
		fScale += 0.01f;
	}
	if (GetKeyState(VK_SUBTRACT) & 0x80)
	{
		fScale -= 0.01f;
		if (fScale <= 0.01f)
			fScale = 0.01f;
	}



}

void CScene3::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
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
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene3::Draw3DSGrid(float width, float length)
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



void CScene3::DrawAxis()
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


void CScene3::CreateRobot(float x, float y, float z)
{
	// Desenha um Robo
	glPushMatrix(); // Nó princilal (inicio)
	glTranslatef(x, y, z);

		// Corpo (inicio)
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glScalef(5.0f, 8.0f, 3.0f);
		glColor3f(1.0f, 0.0f, 0.0f);	// Seta a cor para vermelho
		glutSolidCube(1.0);
		glPopMatrix(); // Corpo (fim)

		// Cabeça (inicio)
		glPushMatrix();
		glTranslatef(0.0f, 5.5f, 0.0f);
		glColor3f(1.0f, 1.0f, 0.0f);	// Seta a cor para amarelo
		glutSolidCube(2.0);
		glPopMatrix(); // Cabeça (fim)

		// Ombro esquerdo (inicio)
		glPushMatrix();
		glTranslatef(-2.5f, 3.5f, 0.0f);
		glColor3f(0.5f, 0.0f, 0.0f);	// Seta a cor para vermelho
		glutSolidSphere(1.0, 10, 10);

			// Braço esquerdo (inicio)
			glPushMatrix();
			glRotatef(-rotArms, 0.0f, 0.0f, 1.0f);
			glTranslatef(-2.0f, 0.0f, 0.0f);
			glScalef(5.0f, 1.0f, 1.0f);
			glColor3f(1.0f, 1.0f, 0.0f);	// Seta a cor para amarelo
			glutSolidCube(1.0f);
			glPopMatrix(); // Braço esquerdo (fim)

		glPopMatrix(); // Ombro esquerdo (fim)



		// Ombro direito (inicio)
		glPushMatrix();
		// Cria uma matriz de translação
		glm::mat4 m(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					2.5f, 3.5f, 0.0f, 1.0f);
		// Multiplica a matriz atual (identidade) com a nova matriz (m)
		// para gerar a translação do objeto
		glMultMatrixf(&m[0][0]);
		glColor3f(0.5f, 0.0f, 0.0f);	// Seta a cor para vermelho
		glutSolidSphere(1.0, 10, 10);


			// Braço direito (inicio)
			glPushMatrix();
			// Cria uma matriz de rotação
			glm::mat4 m2(glm::cos(glm::radians(rotArms)), glm::sin(glm::radians(rotArms)),		0.0f,	0.0f,
						-glm::sin(glm::radians(rotArms)), glm::cos(glm::radians(rotArms)),		0.0f,	0.0f,
													0.0f,							 0.0f,		1.0f,	0.0f,
													0.0f,							 0.0f,		0.0f,	1.0f);
			// Multiplica a matriz atual (ombro direito) com a nova matriz (m2)
			// para gerar a rotação do objeto
			glMultMatrixf(&m2[0][0]);
			glTranslatef(2.0f, 0.0f, 0.0f);
			glScalef(5.0f, 1.0f, 1.0f);
			glColor3f(1.0f, 1.0f, 0.0f);	// Seta a cor para amarelo
			glutSolidCube(1.0f);
			glPopMatrix();  // Braço direito (fim)

		glPopMatrix(); // Ombro direito (fim)

	glPopMatrix();  // Nó princilal (fim)





	if (rotCounterClockWise == true)
	{
		rotArms -= 1.0f;
		if (rotArms <= -80.0f)
			rotCounterClockWise = !rotCounterClockWise;
	}
	else
	{
		rotArms += 1.0f;
		if (rotArms >= 80.0f)
			rotCounterClockWise = !rotCounterClockWise;
	}
}