
#include "stdafx.h"
#include "SceneMgr.h"
#include "Renderer.h"
#include <random>

std::default_random_engine dre;

std::uniform_int_distribution<int> uix(-5, 5);
std::uniform_int_distribution<int> uiy(0, 5);

std::uniform_int_distribution<int> posx(-WindowWidth/2 + 30, WindowWidth/2 -30);
std::uniform_int_distribution<int> posy(30, 100);


SceneMgr::SceneMgr() {
	m_Renderer = new Renderer(WindowWidth, WindowHeight);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	m_texImage[0] = m_Renderer->CreatePngTexture("Resource/pic1.png");
	m_texImage[1] = m_Renderer->CreatePngTexture("Resource/pic2.png");
}

SceneMgr::~SceneMgr() {

}


void SceneMgr::BuildObjects() {
	//북
	AddObject(float3(0, WindowHeight / 2 - 150, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	AddObject(float3(-WindowWidth/2 + 50, WindowHeight / 2 - 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	AddObject(float3(+WindowWidth/2 - 50, WindowHeight / 2 - 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	//남
	AddObject(float3(0, -WindowHeight / 2 + 150, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
	AddObject(float3(-WindowWidth/2 + 50, -WindowHeight / 2 + 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
	AddObject(float3(+WindowWidth/2 - 50, -WindowHeight / 2 + 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
}

void SceneMgr::Collision() {
	int damage = 0;
	//오브젝트 끼리의 충돌체크

	for (auto& character : m_CharacterObjects) {
		//캐릭터 - 빌딩
		for (auto& building : m_BuildingObjects) {
			if (character->getTeam() == building->getTeam())
				continue;

			if (character->getOOBB()->collision(*building->getOOBB())) {
				damage = character->getLife();
				character->setminusLife(damage);
				building->setminusLife(damage);
			}
		}

		//캐릭터 - 총알
		for (auto& bullet : m_BulletObjects) {
			if (character->getTeam() == bullet->getTeam())
				continue;

			if (character->getOOBB()->collision(*bullet->getOOBB())) {
				damage = bullet->getLife();
				character->setminusLife(damage);
				bullet->setminusLife(damage);
			}
		}

		//캐릭터 - 화살
		for (auto& arrow : m_ArrowObjects) {
			/*if (character->getID() == arrow->getID())
				continue;*/
			if (character->getTeam() == arrow->getTeam())
				continue;

			if (character->getOOBB()->collision(*arrow->getOOBB())) {
				damage = arrow->getLife();
				character->setminusLife(damage);
				arrow->setminusLife(damage);
			}
		}
	}

	for (auto& building : m_BuildingObjects) {
		//빌당 - 화살
		for (auto& arrow : m_ArrowObjects) {
			if (arrow->getTeam() == building->getTeam())
				continue;
			if (arrow->getOOBB()->collision(*building->getOOBB())) {
				damage = arrow->getLife();
				arrow->setminusLife(damage);
				building->setminusLife(damage);
			}
		}

		//빌딩 - 총알
		for (auto& bullet : m_BulletObjects) {
			if (bullet->getTeam() == building->getTeam())
				continue;
			if (bullet->getOOBB()->collision(*building->getOOBB())) {
				damage = bullet->getLife();
				bullet->setminusLife(damage);
				building->setminusLife(damage);
			}
		}
	}
}

void SceneMgr::Update(float ElapsedTime) {
	DWORD currTime = timeGetTime() *0.001f;

	//1초 간격으로 실행하는 함수
	if (currTime - prevTime[0] > timeLimit[0]) {
		CreateCharacterObjects(TEAM::TEAM_1); //인공지능이 캐릭터를 생성
		prevTime[0] = currTime;
	}

	std::list<Objects*>::iterator iter;

	//캐릭터 오브젝트
	for (iter = m_CharacterObjects.begin(); iter != m_CharacterObjects.end(); )
	{
		//트루가 반환되면 화살 쏘기
		if ((*iter)->Update(ElapsedTime)) {
			float3 pos = (*iter)->getPos();
			float3 dir = (*iter)->getMoveDir();
			AddArrow(pos, dir, (*iter)->getID(), (*iter)->getTeam());
		}

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
		//트루가 반환되면 총알쏘기
		if ((*iter)->Update(ElapsedTime)) {
			float3 dir(float3(uix(dre), uiy(dre), 0));
			float3 pos = (*iter)->getPos();

			if ((*iter)->getTeam() == TEAM::TEAM_1)
				dir.y *= -1;

			AddObject(pos, dir, ObjectType::OBJECT_BULLET, (*iter)->getTeam(), -1);
		}

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
	if (y > 0)
		return;
	DWORD currTime = timeGetTime() *0.001f;
	//플레이어 캐릭터 생성은 2초 쿨타임
	if (currTime - prevTime[1] > timeLimit[1]) {
		AddObject(float3(x, y, 0), float3(uix(dre), uiy(dre), uix(dre)), ObjectType::OBJECT_CHARACTER, TEAM::TEAM_2, -1);
		prevTime[1] = currTime;
	}
}


void SceneMgr::AddObject(float3 pos, float3 dir, int type, TEAM team, int texIndex) {

	Objects* newObject = nullptr;
	float4 color(0, 0, 0, 1);
	switch (type) {
	case ObjectType::OBJECT_BUILDING:
		newObject = new Objects(pos, float4(1, 1, 1, 1), 100, 0, "빌딩",
			float3(1, 1, 1), 0, 500);

		newObject->setType(ObjectType::OBJECT_BUILDING);
		newObject->setTexIndex(texIndex);
		newObject->setTeam(team);
		newObject->setRenderLevel(Render_Level.RENDER_BUILDING);
		newObject->setIsLifeGuage(true);
		m_BuildingObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_CHARACTER:
		switch (team) {
		case TEAM::TEAM_1:
			color.x = 1;
			break;
		case TEAM::TEAM_2:
			color.z = 1;
			break;
		default:
			break;
		}

		newObject = new Objects(pos, color, 30, 0, "캐릭터",
			dir, 300, 100);

		newObject->setType(0.3f);
		newObject->setID(characterID);
		characterID += 1;
		newObject->setTeam(team);
		newObject->setRenderLevel(Render_Level.RENDER_CHARACTER);
		newObject->setIsLifeGuage(true);
		m_CharacterObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_BULLET:
		switch (team) {
		case TEAM::TEAM_1:
			color.x = 1;
			break;
		case TEAM::TEAM_2:
			color.z = 1;
			break;
		default:
			break;
		}

		newObject = new Objects(pos, color, 4, 0, "총알",
			dir, 600, 15);
		
		newObject->setType(ObjectType::OBJECT_BULLET);
		newObject->setTeam(team);
		newObject->setRenderLevel(Render_Level.RENDER_PROJECTILE);
		newObject->setIsProjecttile(true);
		m_BulletObjects.push_back(newObject);
		break;

	default:
		std::cout << "undefined object type - " << type <<std::endl;
		return;
	}
}

void SceneMgr::AddArrow(float3 pos, float3 dir, int id, TEAM team)
{
	Objects* newObject = nullptr;
	float4 color(0, 0, 0, 1);

	switch (team) {
	case TEAM::TEAM_1:
		color.x = 0.5;
		color.y = 0.2;
		color.z = 0.7;
		break;
	case TEAM::TEAM_2:
		color.x = 1;
		color.y = 1;
		break;
	default:
		break;
	}
	newObject = new Objects(pos, color, 4, 0, "화살",
		dir, 100, 10);

	newObject->setType(ObjectType::OBJECT_ARROW);
	newObject->setID(id);
	newObject->setTeam(team);
	newObject->setIsProjecttile(true);
	newObject->setRenderLevel(Render_Level.RENDER_PROJECTILE);
	m_ArrowObjects.push_back(newObject);
}

void SceneMgr::CreateCharacterObjects(TEAM team)
{
	float3 pos(posx(dre), +WindowHeight / 2 - posy(dre), 0);
	float3 dir(uix(dre), uiy(dre), uix(dre));
	dir.y *= -1;

	AddObject(pos, dir, ObjectType::OBJECT_CHARACTER, team, -1);
}