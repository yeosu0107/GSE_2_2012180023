
#include "stdafx.h"
#include "SceneMgr.h"
#include "Renderer.h"
#include <random>

std::default_random_engine dre;
std::uniform_int_distribution<int> ui(-5, 5);
std::uniform_int_distribution<int> pos(-240, 240);


SceneMgr::SceneMgr() {
	m_currObjectscount = 0;
	memset(m_Objects, 0, sizeof(Objects));
	m_Renderer = new Renderer(WindowWidth, WindowHeight);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
}

SceneMgr::~SceneMgr() {
	delete[] m_Objects;
}


void SceneMgr::BuildObjects() {
	AddObject(0, 0, ObjectType::OBJECT_BUILDING);
}

void SceneMgr::Collision() {
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if (!m_Objects[i])
			continue;
		//m_Objects[i]->setCollisionCheck(0);
	}

	/*|| m_Objects[i]->getNowCollision()*/

	//오브젝트 끼리의 충돌체크
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if (!m_Objects[i])
			continue;
		for (int j = 0; j < MAX_OBJECT_COUNT; ++j) {
			if (i == j || !m_Objects[j])
				continue;

			if (m_Objects[i]->getOOBB()->collision(*m_Objects[j]->getOOBB())) {
				switch (m_Objects[i]->getType()) {
				case ObjectType::OBJECT_BUILDING:
					if (m_Objects[j]->getType() == ObjectType::OBJECT_CHARACTER) {
						m_Objects[i]->setminusLife(m_Objects[j]->getLife());

						m_Objects[j]->setminusLife(m_Objects[j]->getLife());
					}
					break;
				case ObjectType::OBJECT_CHARACTER:
					if (m_Objects[j]->getType() == ObjectType::OBJECT_BUILDING) {
						m_Objects[i]->setminusLife(m_Objects[i]->getLife());

						m_Objects[j]->setminusLife(m_Objects[i]->getLife());
					}

					if (m_Objects[j]->getType() == ObjectType::OBJECT_BULLET) {
						m_Objects[i]->setminusLife(m_Objects[j]->getLife());

						m_Objects[j]->setminusLife(m_Objects[j]->getLife());
					}
					break;
				case ObjectType::OBJECT_BULLET:
					if (m_Objects[j]->getType() == ObjectType::OBJECT_CHARACTER) {
						m_Objects[i]->setminusLife(m_Objects[i]->getLife());

						m_Objects[j]->setminusLife(m_Objects[i]->getLife());
					}
					break;
				}
			}
		}
	}
}

void SceneMgr::Update(float ElapsedTime) {
	DWORD currTime = timeGetTime() *0.001f;
	if (currTime - prevTime > timeCount) {
		AddObject(0, 0, ObjectType::OBJECT_BULLET);
		prevTime = currTime;
	}
	
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if (m_Objects[i] != nullptr) {
			m_Objects[i]->Update(ElapsedTime);

			if (!m_Objects[i]->getLive()) {
				DeleteObject(i);
			}
		}
		
	}
	Collision();
	
}

void SceneMgr::Render() {
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if(m_Objects[i])
			m_Objects[i]->Render(*m_Renderer);
	}
}

void SceneMgr::MouseInput(int x, int y) {
	AddObject(x, y, ObjectType::OBJECT_CHARACTER);
}


void SceneMgr::AddObject(int x, int y, int type) {
	while (m_Objects[m_currObjectscount] != nullptr) {
		m_currObjectscount++;
		if (m_currObjectscount >= MAX_OBJECT_COUNT)
			m_currObjectscount = 0;
	}

	Objects* newObject = nullptr;

	switch (type) {
	case ObjectType::OBJECT_BUILDING:
		newObject = new Objects(float3(x, y, 0), float4(1, 1, 0, 1), 50, 0, "빌딩",
			float3(ui(dre), ui(dre), 0), 0, 500);
		newObject->setType(ObjectType::OBJECT_BUILDING);
		break;
	case ObjectType::OBJECT_CHARACTER:
		newObject = new Objects(float3(x, y, 0), float4(1, 1, 1, 1), 10, 0, "캐릭터",
			float3(ui(dre), ui(dre), 0), 100, 10);
		newObject->setType(ObjectType::OBJECT_CHARACTER);
		break;
	case ObjectType::OBJECT_ARROW:
		newObject = new Objects(float3(x, y, 0), float4(0, 1, 0, 1), 2, 0, "화살",
			float3(ui(dre), ui(dre), 0), 100, 10);
		newObject->setType(ObjectType::OBJECT_ARROW);
		break;
	case ObjectType::OBJECT_BULLET:
		newObject = new Objects(float3(x, y, 0), float4(1, 0, 0, 1), 2, 0, "총알",
			float3(ui(dre), ui(dre), 0), 300, 20);
		newObject->setType(ObjectType::OBJECT_BULLET);
		break;
	default:
		
		return;
	}
	
	m_Objects[m_currObjectscount] = newObject;
	//m_currObjectscount += 1;
}

void SceneMgr::DeleteObject(int index)
{
	if (m_Objects[index] != nullptr) {
		m_Objects[index] = nullptr;
	}
}