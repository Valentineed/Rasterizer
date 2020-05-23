// HelloGLUT.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <chrono>

#include "Rasterizer.h"

static Rasterizer::Scene* g_scene;
static Rasterizer::Screen* g_screen;
static Rasterizer::Screen* g_buffer;
static Rasterizer::Camera* g_camera;

static bool g_rotationDown = false;
static bool g_rotationUp = false;
static bool g_rotationLeft = false;
static bool g_rotationRight = false;
static bool g_translationFront = false;
static bool g_translationBack = false;
static bool g_translationLeft = false;
static bool g_translationRight = false;
static bool g_translationUp = false;
static bool g_translationDown = false;

static bool g_wireframe = false;
static bool g_culling = false;
static bool g_light = true;

std::chrono::high_resolution_clock::time_point m_lastFrameTime = std::chrono::high_resolution_clock::now();
float elapse = 0.f;
float test = 0.f;
int g_frameCount = 0;

void OnUpdate(int i)
{
	float deltaTime;
	{
		using namespace std::chrono;
		high_resolution_clock::time_point now = high_resolution_clock::now();
		duration<float> deltaDuration = duration_cast<duration<float>>(now - m_lastFrameTime);
		m_lastFrameTime = now;
		deltaTime = deltaDuration.count();

#ifdef _DEBUG
		if (g_frameCount >= 10)
		{
			std::cout << "Debug FPS: " << 1.f / (elapse / 10.f) << "\n";
			elapse = 0.f;
			g_frameCount = 0;
		}
#else
		if (g_frameCount >= 100)
		{
			std::cout << "Release FPS: " << 1.f / (elapse / 100.f) << "\n";
			elapse = 0.f;
			g_frameCount = 0;
		}
#endif // _DEBUG
	}

	elapse += deltaTime;

	g_camera->Rotate({ (g_rotationUp - g_rotationDown) * deltaTime * 90,
						(g_rotationRight - g_rotationLeft)* deltaTime * 90,
						0.f});
	g_camera->Translate({ (g_translationRight - g_translationLeft) * deltaTime * 2,
							(g_translationUp - g_translationDown) * deltaTime * 2,
							(g_translationBack - g_translationFront) * deltaTime * 2 });


	test += deltaTime;
	
	auto it = g_scene->begin();
	g_scene->begin()->ResetTransformation();
	//g_scene->begin()->Scale({ 2.f, 2.5f, 1.f });
	//g_scene->begin()->Rotate({ 0.f, test * 90, 0.f });
	g_scene->begin()->Translate({ 2.f * cosf(test), 0.f, -2.f * sinf(test) });

	glutPostRedisplay();
	glutTimerFunc(0, OnUpdate, 0);
}

void OnDisplay()
{
	g_frameCount++;

	Rasterizer::Rasterizer::RenderScene(*g_scene, *g_screen, *g_camera, g_wireframe, g_culling, g_light);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, g_screen->GetWidth(), g_screen->GetHeight(), 0, GL_RGBA, GL_FLOAT, g_screen->GetPixelBuffer());

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.f, 1.f);
		glVertex2f(-1.f, 1.f);

		glTexCoord2f(1.f, 1.f);
		glVertex2f(1.f, 1.f);

		glTexCoord2f(1.f, 0.f);
		glVertex2f(1.f, -1.f);

		glTexCoord2f(0.f, 0.f);
		glVertex2f(-1.f, -1.f);
	}
	glEnd();

	glutSwapBuffers();
}

void OnReshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	g_screen->Resize(width, height);
	g_buffer->Resize(width * 4, height * 4);
}

void OnKeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		g_translationFront = true;
		break;
	case 's':
		g_translationBack = true;
		break;
	case 'a':
		g_translationLeft = true;
		break;
	case 'd':
		g_translationRight = true;
		break;
	case 'e':
		g_translationUp = true;
		break;
	case 'q':
		g_translationDown = true;
		break;
	case 27: // 'ESC'
		exit(0);
		break;
	default:
		break;
	}
}

void OnKeyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		g_translationFront = false;
		break;
	case 's':
		g_translationBack = false;
		break;
	case 'a':
		g_translationLeft = false;
		break;
	case 'd':
		g_translationRight = false;
		break;
	case 'e':
		g_translationUp = false;
		break;
	case 'q':
		g_translationDown = false;
		break;
	default:
		break;
	}
}

void OnSpecialDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		g_wireframe = !g_wireframe;
		break;
	case GLUT_KEY_F2:
		g_culling = !g_culling;
		break;
	case GLUT_KEY_F3:
		g_light = !g_light;
		break;
	case GLUT_KEY_DOWN:
		g_rotationDown = true;
		break;
	case GLUT_KEY_UP:
		g_rotationUp = true;
		break;
	case GLUT_KEY_RIGHT:
		g_rotationLeft = true;
		break;
	case GLUT_KEY_LEFT:
		g_rotationRight = true;
		break;
	default:
		break;
	}
}

void OnSpecialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		g_rotationDown = false;
		break;
	case GLUT_KEY_UP:
		g_rotationUp = false;
		break;
	case GLUT_KEY_RIGHT:
		g_rotationLeft = false;
		break;
	case GLUT_KEY_LEFT:
		g_rotationRight = false;
		break;
	default:
		break;
	}
}

void InitOpenGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("rasterize me");

	//Defini la couleur de Clear pour clear la fenetre
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glutKeyboardFunc(&OnKeyDown);
	glutKeyboardUpFunc(&OnKeyUp);
	glutSpecialFunc(&OnSpecialDown);
	glutSpecialUpFunc(&OnSpecialUp);
	glutReshapeFunc(OnReshape);
	glutDisplayFunc(OnDisplay);
}

int main(int argc, char** argv)
{
	InitOpenGL(argc, argv);

	g_screen = new Rasterizer::Screen();
	g_buffer = new Rasterizer::Screen();
	g_scene = new Rasterizer::Scene();
	g_camera = new Rasterizer::Camera();
	g_camera->SetPerspectiveView(1024, 768, 1.f, 10.f, 120.f);
	g_camera->SetPosition({ 0.f, 0.f, 5.f });
	//g_scene->AddLight({ 0.f, 0.f, -4.f, 0.6f, 0.4f, 0.3f });
	g_scene->AddLight({ 0.f, 0.f, 0.5f, 0.4f, 0.4f, 0.2f });

	Rasterizer::Mesh* obj = Rasterizer::Mesh::CreateFromFile("Asset/diamond.obj");
	Rasterizer::Mesh* cube = Rasterizer::Mesh::CreateCube();
	Rasterizer::Mesh* sphere = Rasterizer::Mesh::CreateSphere(12,24);
	Rasterizer::Mesh mesh;

	mesh.AddNormal(1.f, 0.f, 0.f);
	mesh.AddTexel(0.f, 0.f);

	mesh.AddCoordinate(-2.f, 0.f, -5.f);//
	mesh.AddCoordinate(-2.f, 2.f, -8.f);
	mesh.AddCoordinate(-2.f, -2.f, -8.f);
	mesh.AddTriangle({ 0, { 1.f, 0, 0 } }, { 1, { 0, 1.f, 0 } }, { 2, { 0, 0, 1.f } });

	mesh.AddCoordinate(2.f, 0.f, -5.f);//
	mesh.AddCoordinate(2.f, 2.f, -8.f);
	mesh.AddCoordinate(2.f, -2.f, -8.f);
	mesh.AddTriangle({ 3, { 1.f, 0, 0 } }, { 4, { 0, 1.f, 0 } }, { 5, { 0, 0, 1.f } });

	mesh.AddCoordinate(0.f, 0.f, 0.f);
	mesh.AddTriangle({ 6 }, { 0, { 1.f, 0, 0 } }, { 3, { 1.f, 0, 0 } });

	mesh.AddCoordinate(0.f,-2.f, -5.f);//
	mesh.AddCoordinate(0.f, 2.f, -5.f);//
	mesh.AddTriangle({ 6 }, { 7, { 1.f, 0, 0 } }, { 8, { 1.f, 0, 0 } });

	//g_scene->AddEntity(obj);
	g_scene->AddEntity(cube);
	//g_scene->AddEntity(sphere);
	//g_scene->AddEntity({ &mesh, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, 0.5f });

	glutTimerFunc(0, &OnUpdate, 0);
	glutMainLoop();
}