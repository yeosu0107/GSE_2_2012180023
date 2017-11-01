#pragma once
#include "Objects.h"
#include <vector>

const int MAX_OBJECT_COUNT = 1500;

class SceneMgr
{
private:
	Renderer* m_Renderer;
	//std::vector<Objects*> m_Objects;
	Objects* m_Objects[MAX_OBJECT_COUNT];
	int m_currObjectscount;
	
	float timeCount = 0.5f;
	DWORD prevTime = 0;

	enum ObjectType {OBJECT_BUILDING=0, OBJECT_CHARACTER, 
						OBJECT_BULLET, OBJECT_ARROW};

public:
	SceneMgr();
	~SceneMgr();
	
	void BuildObjects();
	void Render();
	void Collision();
	void Update(float ElapsedTime);
	
	void MouseInput(int x, int y);

	void AddObject(int x, int y, int type);
	void DeleteObject(int index);
};