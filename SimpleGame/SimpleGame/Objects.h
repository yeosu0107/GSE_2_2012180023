#pragma once
#include <math.h>

const int namebuff = 10;

class Renderer;

enum TEAM {TEAM_1=0, TEAM_2, NONE};

class RENDER_LEVEL
{
public:
	const float RENDER_GOD					= 0.0f;
	const float RENDER_BUILDING				= 0.1f;
	const float RENDER_CHARACTER			= 0.2f;
	const float RENDER_PROJECTILE			= 0.3f;
};

class Objects
{
protected:
	float3 m_Pos;		//위치
	float4 m_Color;	//색깔
	float4 m_teamColor; //팀칼라
	float m_Size;		//크기
	float m_Weight;	//무게
	
	UINT m_texIndex;		//텍스쳐
	bool	m_IsAnimate;		//애니메이션이 있는가?
	int		m_CurrXSeq;		//현재x시퀸스
	int		m_CurrYSeq;		//현재y시퀸스
	int		m_MaxXSeq;		//최대x시퀸스
	int		m_MaxYSeq;		//최대y시퀸스
	float	m_aniTime;

	bool	m_IsPaticle;			//파티클이 있는가
	UINT	m_PaticleTexIndex;	//파티클 텍스쳐
	float	m_paticleTime;

	float m_RenderLevel; //랜더링 순서 (작을수록 위에 그려짐)
	bool m_isProjecttile;

	char* m_Name;	//이름
	int m_id;
	
	float3 m_moveDir;		//이동방향 (x,y,z)
	float m_moveSpeed;	//이동속도

	int now_crash_count;	//몇번 충돌했나?
	OOBB* m_oobb = nullptr;;	//OOBB박스

	bool m_Live;		//게임상에 표현되는 여부 (true - 표현해라, false -  지워진 상태)
	int m_LifeTime;   //최대 생존 시간

	bool m_isLifeGuage; //라이프게이지 표시 유무
	float m_LifeGuage;	//라이프게이지
	int m_FullLife;		//최대 HP
	int m_Life;			//현재 HP
	char m_buf[10];	//라이프게이지 텍스트

	int m_type;			//오브젝트 타입
	TEAM m_Team;		//팀

	DWORD m_PrevTime; 
	float		m_LimitTime;

public:
	Objects();
	Objects(float x, float y, float z, float r, float g, float b, float a, float size, float weight, 
		char* name, float mx, float my, float mz, float speed, int life);
	//좌표, RGB , 사이즈, 질량, 이름, 이동방향, 속도, 생명력
	Objects(float3 pos, float4 color, float size, float weight, char* name, float3 dir, float speed, int life);
	//좌표, RGB , 사이즈, 질량, 이름, 이동방향, 속도, 생명력
	~Objects();

	//변수값 저장
	void setPos(float, float, float);
	void setPos(float3);
	void setColor(float, float, float, float);
	void setColor(float4);
	void setSize(float size) { 
		m_Size = size; 
		if (m_oobb)
			delete m_oobb;
		m_oobb = new OOBB(m_Pos, m_Size);
	}
	void setWeight(float weight) { m_Weight = weight; }
	void setType(int type) { m_type = type; }
	void setIsProjecttile(bool tmp) { m_isProjecttile = tmp; }

	void setLive(bool live) { m_Live = live; }
	void setName(char* name) { m_Name = name; }

	void setSpeed(float speed) { m_moveSpeed = speed; }
	void setMoveDir(float, float, float);
	void setMoveDir(float3);

	void setminusLife(int tmp);
	void setCollisionCheck(int tmp) { now_crash_count = tmp; }
	void setID(int tmp) { m_id = tmp; }
	void setTexIndex(int tmp) { 	m_texIndex = tmp; }
	void setTexSeq(int x, int y) {
		m_IsAnimate = true;
		m_CurrXSeq = 0;
		m_CurrYSeq = 0;
		m_MaxXSeq = x;
		m_MaxYSeq = y;
	}
	void setPaticle(UINT tex) {
		m_IsPaticle = true;
		m_PaticleTexIndex = tex;
		m_paticleTime = 0.0f;
	}

	void setTeam(TEAM tmp) { 
		m_Team = tmp; 
		if (tmp == TEAM::TEAM_1)
			m_teamColor = float4(1, 0, 0, 1);
		else if (tmp == TEAM::TEAM_2)
			m_teamColor = float4(0, 0, 1, 1);
	}
	void setRenderLevel(float tmp) { m_RenderLevel = tmp; }
	void setIsLifeGuage(bool tmp) { m_isLifeGuage = tmp; }

	//변수 값 불러오기
	float3 getPos() const { return m_Pos; }
	float4 getColor() const { return m_Color; }
	float getSize() const { return m_Size; }
	float getWeight() const { return m_Weight; }
	int getType() const { return m_type; }

	bool getLive() const { return m_Live; }
	int getLife() const { return m_Life; }
	char* getName() const { return m_Name; }

	float getSpeed() const { return m_moveSpeed; }
	float3 getMoveDir() const { return m_moveDir; }

	OOBB* getOOBB() const { return m_oobb; }
	int getCollisonCheck() const { return now_crash_count; }
	int getID() const { return m_id; }
	int getTexIndex() const { return m_texIndex; }
	TEAM getTeam() const { return m_Team; }

	//bool getNowCollision()const { return now_crash; }

	//오브젝트 제어
	void Move(float ElapsedTime);								//이동방향으로 이동속도만큼 이동
	void Move(float ElapsedTime, float3 moveValue);		//속도벡터만큼 이동


	virtual void Animate();					//애니메이트
	virtual void CrashCheck();				//충돌체크
	

	virtual void OnPrepareRender();		//랜더링 전에 동작해야 할 것들
	virtual void Render(Renderer& renderer);					//랜더링

	virtual bool Update(float ElapsedTime);					//업데이트
};
