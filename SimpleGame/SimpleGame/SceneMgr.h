#pragma once
#include "Objects.h"

const int MAX_OBJECT_COUNT = 1500;

class SceneMgr
{
private:
	Renderer* m_Renderer;
	Objects* m_Objects[MAX_OBJECT_COUNT];
	int m_currObjectscount;
	

public:
	SceneMgr();
	~SceneMgr();
	
	void BuildObjects();
	void Render();
	void Collision();
	void Update(float ElapsedTime);

	void Add(int x, int y);
};