#pragma once
#include "Objects.h"
#include <list>

const int MAX_OBJECT_COUNT = 1500;
const int MAX_TEXTURE_COUNT = 6;

class SceneMgr
{
private:
	Renderer* m_Renderer;

	std::list<Objects*> m_BuildingObjects;
	std::list<Objects*> m_BulletObjects;
	std::list<Objects*> m_CharacterObjects;
	std::list<Objects*> m_ArrowObjects;

	UINT m_texImage[MAX_TEXTURE_COUNT];

	float timeLimit[2] = { 1.0f, 2.0f };
	DWORD prevTime[2] = { 0, 0 };

	enum ObjectType {OBJECT_BUILDING=0, OBJECT_CHARACTER, 
						OBJECT_BULLET, OBJECT_ARROW};

	int characterID = 0;
	RENDER_LEVEL	Render_Level;
public:
	SceneMgr();
	~SceneMgr();
	
	void BuildObjects();
	void Render();
	void Collision();
	void Update(float ElapsedTime);
	
	void MouseInput(int x, int y);

	void AddObject(float3 pos, float3 dir, int type, TEAM team, int texIndex);
	void AddArrow(float3 pos, float3 dir, int id, TEAM team);

	void CreateCharacterObjects(TEAM team);
};