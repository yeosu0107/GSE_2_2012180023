/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "Objects.h"

Renderer *g_Renderer = NULL;
std::vector<Objects*> g_Objects;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	/*g_Object->Update();
	g_Object->Render(*g_Renderer);*/
	for (Objects* object : g_Objects) {
		object->Update();
		object->Render(*g_Renderer);
	}

	glutSwapBuffers();
}

void Idle(void)
{
	//g_Object->Update();
	for (Objects* object : g_Objects) {
		object->Update();
	}
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	//RenderScene();
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		int xPos = x-WindowWidth / 2;
		int yPos = WindowHeight / 2-y;
		Objects   *g_Object = nullptr;
		g_Object = new Objects(xPos, yPos, 0, 1, 0, 0, 1, 10, 1, "임시", 1, 1, 0, 0.1f);
		g_Objects.emplace_back(g_Object);
	}
}

void KeyInput(unsigned char key, int x, int y)
{
	//RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	//RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer
	g_Renderer = new Renderer(WindowWidth, WindowHeight);
	if (!g_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	//g_Object = new Objects(100, -30, 0, 1, 0, 0, 1, 10, 1, "임시", 1, 1, 0, 0.1f);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_Renderer;
	g_Objects.clear();
    return 0;
}

