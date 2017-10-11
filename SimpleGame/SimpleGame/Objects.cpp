#include "stdafx.h"
#include "Objects.h"
#include "Renderer.h"


Objects::Objects() : m_Pos(0, 0, 0), m_Size(0), m_Weight(0), m_Color(0, 0, 0, 0),
	m_Live(true), m_moveDir(0,0,0), m_moveSpeed(0)
{
	m_Name = new char[namebuff];
}

Objects::Objects(float x, float y, float z, float r, float g, float b, float a, float size, float weight,
	char* name, float mx, float my, float mz, float speed) : m_Pos(x,y,z), m_Color(r,g,b,a), m_Size(size), m_Weight(weight),
		m_Live(true), m_moveDir(mx, my, mz), m_moveSpeed(speed)
{
	m_Name = name;
	/*m_Name = new char[namebuff];
	strncpy(m_Name, name, namebuff);*/
}
Objects::Objects(float3 pos, float4 color, float size, float weight, char* name, float3 dir, float speed) :
	m_Pos(pos), m_Color(color), m_Size(size), m_Weight(weight), m_Live(true), m_moveDir(dir), m_moveSpeed(speed)
{
	m_Name = name;
	/*m_Name = new char[namebuff];
	strncpy(m_Name, name, namebuff);*/
}

Objects::~Objects()
{
	delete[] m_Name;
}


void Objects::setPos(float x, float y, float z)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;
}

void Objects::setPos(float3 pos)
{
	m_Pos = pos;
}

void Objects::setColor(float x, float y, float z, float w)
{
	m_Color.x = x;
	m_Color.y = y;
	m_Color.z = z;
	m_Color.w = w;
}

void Objects::setColor(float4 color)
{
	m_Color = color;
}

void Objects::setMoveDir(float x, float y, float z)
{
	m_moveDir.x = x;
	m_moveDir.y = y;
	m_moveDir.z = z;
}

void Objects::setMoveDir(float3 moveDir)
{
	m_moveDir = moveDir;
}

void Objects::Move()
{
	float3 moveValue = m_moveDir;
	moveValue*m_moveSpeed;
	m_Pos += moveValue;
	//printf("%f %f %f\n", m_Pos.x, m_Pos.y, m_Pos.z);
}

void Objects::Move(float3 moveValue)
{
	m_Pos += moveValue;
}

void Objects::Animate()
{

}

void Objects::CrashCheck()
{
	if (m_Pos.x<-WindowWidth / 2 || m_Pos.x>WindowWidth/2)
		m_moveDir.x *= -1;
	if (m_Pos.y<-WindowHeight / 2 || m_Pos.y>WindowHeight/2)
		m_moveDir.y *= -1;
}

void Objects::OnPrepareRender()
{

}

void Objects::Render(Renderer& g_Renderer)
{
	OnPrepareRender();
	g_Renderer.DrawSolidRect(m_Pos.x, m_Pos.y, m_Pos.z, m_Size, 
		m_Color.x, m_Color.y, m_Color.z, m_Color.w);
}

void Objects::Update()
{
	Animate();
	CrashCheck();
	Move();
}