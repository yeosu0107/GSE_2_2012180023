
#include "stdafx.h"
#include "SceneMgr.h"
#include <random>

std::default_random_engine dre;
std::uniform_int_distribution<int> ui(-10, 10);
std::uniform_int_distribution<int> pos(-240, 240);


SceneMgr::SceneMgr() {
	m_currObjectscount = 0;
}

SceneMgr::~SceneMgr() {
	delete m_Objects;
}


void SceneMgr::BuildObjects() {
	/*for (int i = 0; i < 2; ++i) {
		Add(pos(dre), pos(dre));
	}
	m_currObjectscount = 2;*/
}

void SceneMgr::Collision() {
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if (!m_Objects[i])
			continue;
		m_Objects[i]->setCollisionCheck(false);
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

				m_Objects[i]->ColorChange();
				m_Objects[j]->ColorChange();

				/*float3 i_moveDir = m_Objects[i]->getMoveDir();
				float3 j_moveDir = m_Objects[j]->getMoveDir();

				m_Objects[i]->setMoveDir(i_moveDir*-1);
				m_Objects[j]->setMoveDir(j_moveDir*-1);*/
				//printf("Crash! %d %d\n", i, j);
			}
		}
	}
}

void SceneMgr::Update() {
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if(m_Objects[i])
			m_Objects[i]->Update();
	}
	Collision();
}

void SceneMgr::Render(Renderer& g_Renderer) {
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i) {
		if(m_Objects[i])
			m_Objects[i]->Render(g_Renderer);
	}
}




void SceneMgr::Add(int x, int y) {
	if (m_currObjectscount >= MAX_OBJECT_COUNT)
		m_currObjectscount = 0;

	Objects* newObject = new Objects(float3(x, y, 0), float4(1, 1, 1, 1), 10, 0, "임시",
		float3(ui(dre), ui(dre), 0), 0.1f);

	m_Objects[m_currObjectscount] = newObject;
	m_currObjectscount += 1;
}