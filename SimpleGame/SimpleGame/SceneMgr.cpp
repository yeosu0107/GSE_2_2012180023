
#include "stdafx.h"
#include "SceneMgr.h"
#include "Renderer.h"
#include <random>

std::default_random_engine dre;
std::uniform_int_distribution<int> ui(-5, 5);
std::uniform_int_distribution<int> pos(-240, 240);


SceneMgr::SceneMgr() {
	m_Renderer = new Renderer(WindowWidth, WindowHeight);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	m_texImage[0] = m_Renderer->CreatePngTexture("Resource/pic1.png");
}

SceneMgr::~SceneMgr() {

}


void SceneMgr::BuildObjects() {
	AddObject(float3(0,0,0), float3(0,0,0), ObjectType::OBJECT_BUILDING);
}

void SceneMgr::Collision() {
	int damage = 0;
	//오브젝트 끼리의 충돌체크

	for (auto& character : m_CharacterObjects) {
		//캐릭터 - 빌딩
		for (auto& building : m_BuildingObjects) {
			if (character->getOOBB()->collision(*building->getOOBB())) {
				damage = character->getLife();
				character->setminusLife(damage);
				building->setminusLife(damage);
			}
		}

		//캐릭터 - 총알
		for (auto& bullet : m_BulletObjects) {
			if (character->getOOBB()->collision(*bullet->getOOBB())) {
				damage = bullet->getLife();
				character->setminusLife(damage);
				bullet->setminusLife(damage);
			}
		}

		//캐릭터 - 화살
		for (auto& arrow : m_ArrowObjects) {
			if (character->getID() == arrow->getID())
				continue;

			if (character->getOOBB()->collision(*arrow->getOOBB())) {
				damage = arrow->getLife();
				character->setminusLife(damage);
				arrow->setminusLife(damage);
			}
		}
	}

	for (auto& arrow : m_ArrowObjects) {
		//빌딩 - 화살
		for (auto& building : m_BuildingObjects) {
			if (arrow->getOOBB()->collision(*building->getOOBB())) {
				damage = arrow->getLife();
				arrow->setminusLife(damage);
				building->setminusLife(damage);
			}
		}
	}
}

void SceneMgr::Update(float ElapsedTime) {
	DWORD currTime = timeGetTime() *0.001f;

	if (currTime - prevTime > timeCount) {
		float3 pos(0, 0, 0);
		float3 dir(float3(ui(dre), ui(dre), 0));
		float3 shootPoint(0, -20, 0);

		AddObject(pos, dir, ObjectType::OBJECT_BULLET);

		for (auto& character : m_CharacterObjects) {
			pos = character->getPos();
			dir = character->getMoveDir();

			AddArrow(pos, dir, character->getID());
		}

		prevTime = currTime;
	}
	
	std::list<Objects*>::iterator iter;

	//캐릭터 오브젝트
	for (iter = m_CharacterObjects.begin(); iter != m_CharacterObjects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live가 False인 객체 삭제
			m_CharacterObjects.erase(iter++);
		}
		else {
			iter++;
		}
	}
	//빌딩 오브젝트
	for (iter = m_BuildingObjects.begin(); iter != m_BuildingObjects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live가 False인 객체 삭제
			m_BuildingObjects.erase(iter++);
		}
		else {
			iter++;
		}
	}
	//총알 오브젝트
	for (iter = m_BulletObjects.begin(); iter != m_BulletObjects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live가 False인 객체 삭제
			m_BulletObjects.erase(iter++);
		}
		else {
			iter++;
		}
	}
	//화살 오브젝트
	for (iter = m_ArrowObjects.begin(); iter != m_ArrowObjects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live가 False인 객체 삭제
			m_ArrowObjects.erase(iter++);
		}
		else {
			iter++;
		}
	}

	//오브젝트 충돌체크
	Collision();
}

void SceneMgr::Render() {
	for (auto& character : m_CharacterObjects) {
		character->Render(*m_Renderer);
	}
	for (auto& building : m_BuildingObjects) {
		building->Render(*m_Renderer);
	}
	for (auto& bullet : m_BulletObjects) {
		bullet->Render(*m_Renderer);
	}
	for (auto& arrow : m_ArrowObjects) {
		arrow->Render(*m_Renderer);
	}
}

void SceneMgr::MouseInput(int x, int y) {
	float3 dir(float3(ui(dre), ui(dre), 0));
	float3 pos(x, y, 0);
	AddObject(pos, dir, ObjectType::OBJECT_CHARACTER);
}


void SceneMgr::AddObject(float3 pos, float3 dir, int type) {

	Objects* newObject = nullptr;

	switch (type) {
	case ObjectType::OBJECT_BUILDING:
		newObject = new Objects(pos, float4(1, 1, 0, 1), 50, 0, "빌딩",
			float3(ui(dre), ui(dre), 0), 0, 500);

		newObject->setType(ObjectType::OBJECT_BUILDING);
		newObject->setTexIndex(m_texImage[0]);

		m_BuildingObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_CHARACTER:
		newObject = new Objects(pos, float4(1, 1, 1, 1), 10, 0, "캐릭터",
			float3(ui(dre), ui(dre), 0), 300, 10);

		newObject->setType(ObjectType::OBJECT_CHARACTER);
		newObject->setID(characterID);
		characterID += 1;
		

		m_CharacterObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_BULLET:
		newObject = new Objects(pos, float4(1, 0, 0, 1), 2, 0, "총알",
			float3(ui(dre), ui(dre), 0), 300, 20);
		
		newObject->setType(ObjectType::OBJECT_BULLET);

		m_BulletObjects.push_back(newObject);
		break;

	//case ObjectType::OBJECT_ARROW:
	//	newObject = new Objects(pos, float4(0, 1, 0, 1), 2, 0, "화살",
	//		float3(ui(dre), ui(dre), 0), 100, 10);

	//	newObject->setType(ObjectType::OBJECT_ARROW);

	//	m_ArrowObjects.push_back(newObject);
	//	break;

	default:
		return;
	}
}

void SceneMgr::AddArrow(float3 pos, float3 dir, int id)
{
	Objects* newObject = nullptr;
	newObject = new Objects(pos, float4(0, 1, 0, 1), 2, 0, "화살",
		float3(ui(dre), ui(dre), 0), 100, 10);

	newObject->setType(ObjectType::OBJECT_ARROW);
	newObject->setID(id);

	m_ArrowObjects.push_back(newObject);
}