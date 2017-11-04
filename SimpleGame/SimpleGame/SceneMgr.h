#pragma once
#include "Objects.h"
#include <list>

const int MAX_OBJECT_COUNT = 1500;

class SceneMgr
{
private:
	Renderer* m_Renderer;

	std::list<Objects*> m_BuildingObjects;
	std::list<Objects*> m_BulletObjects;
	std::list<Objects*> m_CharacterObjects;
	std::list<Objects*> m_ArrowObjects;

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
};