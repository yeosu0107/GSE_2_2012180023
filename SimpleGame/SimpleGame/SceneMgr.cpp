
#include "stdafx.h"
#include "SceneMgr.h"
#include "Renderer.h"

#include <random>

std::default_random_engine dre;

std::uniform_int_distribution<int> uix(-5, 5);
std::uniform_int_distribution<int> uiy(3, 5);

std::uniform_int_distribution<int> posx(-WindowWidth/2 + 50, WindowWidth/2 - 50);
std::uniform_int_distribution<int> posy(30, 100);


SceneMgr::SceneMgr() {
	m_Renderer = new Renderer(WindowWidth, WindowHeight);
	
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	m_texImage[0] = m_Renderer->CreatePngTexture("Resource/pic1.png");
	m_texImage[1] = m_Renderer->CreatePngTexture("Resource/pic2.png");
	m_texImage[2] = m_Renderer->CreatePngTexture("Resource/back.png");
	m_texImage[3] = m_Renderer->CreatePngTexture("Resource/tmp2.png");
	m_texImage[4] = m_Renderer->CreatePngTexture("Resource/paticle1.png");
	m_texImage[5] = m_Renderer->CreatePngTexture("Resource/paticle2.png");
	m_texImage[6] = m_Renderer->CreatePngTexture("Resource/exp1.png");
	m_texImage[7] = m_Renderer->CreatePngTexture("Resource/exp2.png");
	m_texImage[8] = m_Renderer->CreatePngTexture("Resource/tmp.png");

	m_texClimate = m_Renderer->CreatePngTexture("Resource/snow3.png");

	m_Climatetime = 0.0f;
	m_climateDir = 1.5f;
	m_controlValue = 1.0f;

	m_Sound = new Sound();
	m_soundIndex[0] = m_Sound->CreateSound("./Dependencies/SoundSamples/ophelia.mp3");
	m_soundIndex[1] = m_Sound->CreateSound("./Dependencies/SoundSamples/explosion1.wav");
	m_soundIndex[2] = m_Sound->CreateSound("./Dependencies/SoundSamples/create.mp3");
	m_soundIndex[3] = m_Sound->CreateSound("./Dependencies/SoundSamples/explosion2.wav");
	m_soundIndex[4] = m_Sound->CreateSound("./Dependencies/SoundSamples/hurt.mp3");
	
	m_Sound->PlaySound(m_soundIndex[0], true, 0.2f);

	m_WavingCount = 0;
}
SceneMgr::~SceneMgr() {
	for (int i = 0; i < MAX_SOUND_COUNT; ++i) {
		m_Sound->DeleteSound(m_soundIndex[i]);
	}
	
}


void SceneMgr::BuildObjects() {
	//북
	AddObject(float3(0, WindowHeight / 2 - 150, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	AddObject(float3(-WindowWidth/2 + 60, WindowHeight / 2 - 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	AddObject(float3(+WindowWidth/2 - 60, WindowHeight / 2 - 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_1, m_texImage[0]);
	//남
	AddObject(float3(0, -WindowHeight / 2 + 150, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
	AddObject(float3(-WindowWidth/2 + 60, -WindowHeight / 2 + 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
	AddObject(float3(+WindowWidth/2 - 60, -WindowHeight / 2 + 100, 0), float3(0, 0, 0), ObjectType::OBJECT_BUILDING, TEAM::TEAM_2, m_texImage[1]);
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
				
				m_Sound->PlaySoundW(m_soundIndex[1], false, 0.5f);
				float3 tmp = character->getPos();
				tmp += character->getMoveDir()*character->getSize()*0.5;
				AddObject(tmp, float3(0, 0, 0), ObjectType::OBJECT_SMALLEXP, TEAM::NONE, 0);

				m_WavingCount += 6;
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
				m_Sound->PlaySoundW(m_soundIndex[4], false, 0.5f);
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
				m_Sound->PlaySoundW(m_soundIndex[4], false, 0.5f);
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

				m_Sound->PlaySoundW(m_soundIndex[1], false, 0.5f);
				float3 tmp = arrow->getPos();
				tmp += arrow->getMoveDir()*arrow->getSize()*0.5;
				AddObject(tmp, float3(0, 0, 0), ObjectType::OBJECT_SMALLEXP, TEAM::NONE, 0);
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

				m_Sound->PlaySoundW(m_soundIndex[1], false, 0.5f);
				float3 tmp = bullet->getPos();
				tmp += bullet->getMoveDir()*bullet->getSize()*0.5;
				AddObject(tmp, float3(0, 0, 0), ObjectType::OBJECT_SMALLEXP, TEAM::NONE, 0);
			}
		}
	}
}

void SceneMgr::Update(float ElapsedTime) {
	DWORD currTime = timeGetTime() *0.001f;

	WavingScene();

	//1초 간격으로 실행하는 함수
	if (currTime - prevTime[0] > timeLimit[0]) {
		CreateCharacterObjects(TEAM::TEAM_1); //인공지능이 캐릭터를 생성
		prevTime[0] = currTime;

		if (m_climateDir >= 5.0f || m_climateDir<-5.0f)
			m_controlValue *= -1.0f;
		
	}

	m_Climatetime += 0.01f;
	m_climateDir += m_controlValue*0.1f;

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
			AddObject((*iter)->getPos(), float3(0, 0, 0), ObjectType::OBJECT_BIGEXP, TEAM::NONE, 0);
			m_Sound->PlaySoundW(m_soundIndex[3], false, 1.0f);
			m_BuildingObjects.erase(iter++);
			m_WavingCount += 15;
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

	for (iter = m_Effects.begin(); iter != m_Effects.end(); )
	{
		(*iter)->Update(ElapsedTime);
		if (!(*iter)->getLive()) {//Live가 False인 객체 삭제
			m_Effects.erase(iter++);
		}
		else {
			iter++;
		}
	}

	//오브젝트 충돌체크
	Collision();
}

void SceneMgr::Render() {
	m_Renderer->DrawTexturedRect(0, 0, 0, WindowHeight, 1, 1, 1, 1, m_texImage[2], 0.9f); //배경화면
	m_Renderer->DrawParticleClimate(0, 0, 0, 1, 1, 1, 1, 1, m_climateDir, -0.5f, m_texClimate, m_Climatetime, Render_Level.RENDER_GOD);

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
	for (auto& effect : m_Effects) {
		effect->Render(*m_Renderer);
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
		if (dir.x == 0)
			dir.x = 1;

		switch (team) {
		case TEAM::TEAM_1:
			texIndex = m_texImage[3];
			color.x = 1;
			break;
		case TEAM::TEAM_2:
			texIndex = m_texImage[8];
			color.z = 1;
			break;
		default:
			break;
		}

		newObject = new Objects(pos, color, 70, 0, "캐릭터",
			dir, 200, 100);

		newObject->setType(0.3f);
		newObject->setID(characterID);
		characterID += 1;
		newObject->setTeam(team);

		newObject->setTexIndex(texIndex);
		newObject->setTexSeq(4, 6);

		newObject->setRenderLevel(Render_Level.RENDER_CHARACTER);
		newObject->setIsLifeGuage(true);
		m_CharacterObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_BULLET:
		if (dir.x == 0)
			dir.x = 1;

		newObject = new Objects(pos, color, 5, 0, "총알",
			dir, 200, 15);

		switch (team) {
		case TEAM::TEAM_1:
			color.x = 1;
			newObject->setPaticle(m_texImage[5]);
			break;
		case TEAM::TEAM_2:
			color.z = 1;
			newObject->setPaticle(m_texImage[4]);
			break;
		default:
			break;
		}
		
		newObject->setType(ObjectType::OBJECT_BULLET);
		newObject->setTeam(team);
		newObject->setRenderLevel(Render_Level.RENDER_PROJECTILE);
		newObject->setIsProjecttile(true);
		
		m_BulletObjects.push_back(newObject);
		break;

	case ObjectType::OBJECT_SMALLEXP:
		newObject = new Objects(pos, color, 50, 0, "폭팔1",
			float3(0,0,0), 0, 1);
		newObject->setTexIndex(m_texImage[6]);
		newObject->setTexSeq(4, 4);
		newObject->setRenderLevel(Render_Level.RENDER_GOD);
		m_Effects.push_back(newObject);
		break;
	case ObjectType::OBJECT_BIGEXP:
		newObject = new Objects(pos, color, 100, 0, "폭팔2",
			float3(0, 0, 0), 0, 1);

		newObject->setTexIndex(m_texImage[7]);
		newObject->setTexSeq(5, 3);
		newObject->setRenderLevel(Render_Level.RENDER_GOD);
		m_Effects.push_back(newObject);
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
	newObject = new Objects(pos, color, 10, 0, "화살",
		dir, 400, 10);

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

void SceneMgr::WavingScene()
{
	if (m_WavingCount <= 0) {
		m_Renderer->SetSceneTransform(0, 0, 1, 1);
		return;
	}

	switch (m_WavingCount % 3) {
	case 0:
		m_Renderer->SetSceneTransform(10 * m_WavingCount*0.5, 0, 1, 1);
		break;
	case 1:
		m_Renderer->SetSceneTransform(0, 0, 1, 1);
		break;
	case 2:
		m_Renderer->SetSceneTransform(-10 * m_WavingCount*0.5, 0, 1, 1);
		break;
	}

	m_WavingCount -= 1;
}