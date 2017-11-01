#pragma once
#include <math.h>

const int namebuff = 10;

class Renderer;


class Objects
{
protected:
	float3 m_Pos;		//위치
	float4 m_Color;	//색깔
	float m_Size;		//크기
	float m_Weight;	//무게

	char* m_Name;	//이름
	
	float3 m_moveDir;		//이동방향 (x,y,z)
	float m_moveSpeed;	//이동속도

	int now_crash_count;
	OOBB* m_oobb = nullptr;;

	bool m_Live;		//게임상에 표현되는 여부 (true - 표현해라, false -  지워진 상태)
	int m_LifeTime;
	int m_Life;

	int m_type;

public:
	Objects();
	Objects(float x, float y, float z, float r, float g, float b, float a, float size, float weight, 
		char* name, float mx, float my, float mz, float speed, int life);
	Objects(float3 pos, float4 color, float size, float weight, char* name, float3 dir, float speed, int life);
	~Objects();

	//변수값 저장
	void setPos(float, float, float);
	void setPos(float3);
	void setColor(float, float, float, float);
	void setColor(float4);
	void setSize(float size) { m_Size = size; }
	void setWeight(float weight) { m_Weight = weight; }
	void setType(int type) { m_type = type; }

	void setLive(bool live) { m_Live = live; }
	void setName(char* name) { m_Name = name; }

	void setSpeed(float speed) { m_moveSpeed = speed; }
	void setMoveDir(float, float, float);
	void setMoveDir(float3);

	void setminusLife(int tmp) { m_Life -= tmp; }
	void setCollisionCheck(int tmp) { now_crash_count = tmp; }

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
	//bool getNowCollision()const { return now_crash; }

	//오브젝트 제어
	void Move(float ElapsedTime);								//이동방향으로 이동속도만큼 이동
	void Move(float ElapsedTime, float3 moveValue);		//속도벡터만큼 이동


	virtual void Animate();					//애니메이트
	virtual void CrashCheck();				//충돌체크
	

	virtual void OnPrepareRender();		//랜더링 전에 동작해야 할 것들
	virtual void Render(Renderer& renderer);					//랜더링

	virtual void Update(float ElapsedTime);					//업데이트
};