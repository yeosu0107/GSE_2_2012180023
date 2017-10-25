
#include "stdafx.h"
#include "SceneMgr.h"
#include "Renderer.h"
#include <random>

std::default_random_engine dre;
std::uniform_int_distribution<int> ui(-10, 10);
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
	delete m_Objects;
}


void SceneMgr::BuildObjects() {
	for (int i = 0; i < 50; ++i) {
		Add(pos(dre), pos(dre));
	}
}

void SceneMgr::Collision() {
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if (!m_Objects[i])
			continue;
		m_Objects[i]->setCollisionCheck(false);
		m_Objects[i]->setColor(1, 1, 1, 1);
	}

	//오브젝트 끼리의 충돌체크
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if (!m_Objects[i] || m_Objects[i]->getNowCollision())
			continue;
		for (int j = 0; j < MAX_OBJECT_COUNT; ++j) {
			if ( i == j || !m_Objects[j]
				|| m_Objects[j]->getNowCollision() )
				continue;

			if (m_Objects[i]->getOOBB()->collision(*m_Objects[j]->getOOBB())) {
				//중복충돌체크 방지를 위해 이미 충돌한 객체 체크
				m_Objects[i]->setCollisionCheck(true);
				m_Objects[j]->setCollisionCheck(true);

				m_Objects[i]->setColor(1, 0, 0, 1);
				m_Objects[j]->setColor(1, 0, 0, 1);

				m_Objects[i]->setminusLife(1);
				m_Objects[j]->setminusLife(1);
			}
		}
	}
}

void SceneMgr::Update(float ElapsedTime) {
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if (m_Objects[i] != nullptr) {
			m_Objects[i]->Update(ElapsedTime);

			if (!m_Objects[i]->getLive())
				m_Objects[i] = nullptr;
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




void SceneMgr::Add(int x, int y) {
	if (m_currObjectscount >= MAX_OBJECT_COUNT)
		m_currObjectscount = 0;

	Objects* newObject = new Objects(float3(x, y, 0), float4(1, 1, 1, 1), 10, 0, "임시",
		float3(ui(dre), ui(dre), 0), 10);

	m_Objects[m_currObjectscount] = newObject;
	m_currObjectscount += 1;
}