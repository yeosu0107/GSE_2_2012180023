#pragma once
#include "Objects.h"

const int MAX_OBJECT_COUNT = 1500;

class SceneMgr
{
private:
	Objects* m_Objects[MAX_OBJECT_COUNT];
	int m_currObjectscount;

public:
	SceneMgr();
	~SceneMgr();
	
	void BuildObjects();
	void Render(Renderer& g_Renderer);
	void Collision();
	void Update();

	void Add(int x, int y);
};