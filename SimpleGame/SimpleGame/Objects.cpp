#include "stdafx.h"
#include "Objects.h"
#include "Renderer.h"



Objects::Objects() : m_Pos(0, 0, 0), m_Size(0), m_Weight(0), m_Color(0, 0, 0, 0),
	m_Live(true), m_moveDir(0, 0, 0), m_moveSpeed(0), m_FullLife(0), m_teamColor(1,1,1,1)
{
	m_Name = new char[namebuff];
	now_crash_count = 0;
	m_oobb = new OOBB(m_Pos, m_Size);
	m_LifeTime = 10000;
	m_texIndex = -1;
	m_Team = TEAM::NONE;
	
	m_RenderLevel = 1.0f;

	m_PrevTime = 0;
	m_LimitTime = 1;
	m_LifeGuage = 0;
	m_isLifeGuage = false;
	sprintf_s(m_buf, "%d/%d", (int)m_Life, (int)m_FullLife);

	m_isProjecttile = false;
	m_CurrXSeq = -1;
	m_CurrYSeq = -1;
	m_MaxXSeq = -1;
	m_MaxYSeq = -1;
	m_IsAnimate = false;
	m_aniTime = 0.0f;

	m_IsPaticle = false;
	m_paticleTime = -1.0f;

}


Objects::Objects(float x, float y, float z, float r, float g, float b, float a, float size, float weight,
	char* name, float mx, float my, float mz, float speed, int life) : m_Pos(x,y,z), m_Color(r,g,b,a), m_Size(size), m_Weight(weight),
		m_Live(true), m_moveDir(mx, my, mz), m_moveSpeed(speed), m_Life(life), m_FullLife(life), m_teamColor(1, 1, 1, 1)
{
	m_Name = name;
	now_crash_count = 0;
	m_oobb = new OOBB(m_Pos, m_Size);
	m_texIndex = -1;
	m_Team = TEAM::NONE;
	m_RenderLevel = 1.0f;
	m_CurrXSeq = -1;
	m_CurrYSeq = -1;
	m_MaxXSeq = -1;
	m_MaxYSeq = -1;
	m_IsAnimate = false;
	m_aniTime = 0.0f;

	m_IsPaticle = false;
	m_paticleTime = -1.0f;

	m_PrevTime = 0;
	m_LimitTime = 1;
	m_LifeGuage = 1;
	m_isLifeGuage = false;
	m_isProjecttile = false;

	m_moveDir.normalize();
	sprintf_s(m_buf, "%d/%d", (int)m_Life, (int)m_FullLife);
}

Objects::Objects(float3 pos, float4 color, float size, float weight, char* name, float3 dir, float speed, int life) :
	m_Pos(pos), m_Color(color), m_Size(size), m_Weight(weight), m_Live(true), m_moveDir(dir), m_moveSpeed(speed), m_Life(life), m_FullLife(life), m_teamColor(1, 1, 1, 1)
{
	m_Name = name;
	now_crash_count = 0;
	m_oobb = new OOBB(m_Pos, m_Size);
	m_texIndex = -1;
	m_Team = TEAM::NONE;
	m_RenderLevel = 1.0f;
	m_CurrXSeq = -1;
	m_CurrYSeq = -1;
	m_MaxXSeq = -1;
	m_MaxYSeq = -1;
	m_IsAnimate = false;
	m_aniTime = 0.0f;

	m_IsPaticle = false;
	m_paticleTime = -1.0f;

	m_PrevTime = 0;
	m_LimitTime = 1;
	m_LifeGuage = 1;
	m_isLifeGuage = false;
	m_isProjecttile = false;
	if(m_moveDir.x!=0)
		m_moveDir.normalize();
	sprintf_s(m_buf, "%d/%d", (int)m_Life, (int)m_FullLife);
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

	m_moveDir.normalize();
}

void Objects::setMoveDir(float3 moveDir)
{
	m_moveDir = moveDir;
}

void Objects::Move(float ElapsedTime)
{
	float3 moveValue = m_moveDir*m_moveSpeed*ElapsedTime;
	m_Pos += moveValue;
	

}

void Objects::Move(float ElapsedTime, float3 moveValue)
{
	m_Pos += moveValue;
}

void Objects::Animate()
{
	if (m_IsAnimate) {
		
		/*if (m_moveDir.x > 0) {
			m_aniState = 1;
			m_CurrYSeq = 0;
		}
		else {
			m_aniState = -1;
			m_CurrYSeq = m_divide;
		}*/

		m_aniTime += 0.1f;
		if (m_aniTime >= 0.5f) {
			m_aniTime = 0;
			m_CurrXSeq++;
			if (m_CurrXSeq > m_MaxXSeq) {
				m_CurrXSeq = 0;
				m_CurrYSeq++;

				if (m_aniState == 1) {
					if (m_CurrYSeq > m_divide-1) {
						m_CurrYSeq = 0;

						if (m_Life == 1)
							m_Life = 0; //이펙트 효과 제거
					}
				}
				else if (m_aniState == -1) {
					if (m_CurrYSeq > m_MaxYSeq-1)
						m_CurrYSeq = m_divide;

					if (m_Life == 1)
						m_Life = 0; //이펙트 효과 제거
				}
			}
		}
	}
}

void Objects::CrashCheck()
{
	if (m_isProjecttile) {
		if (m_Pos.x <= -WindowWidth / 2 || m_Pos.x >= WindowWidth / 2)
			m_Live = false;
		if (m_Pos.y <= -WindowHeight / 2 || m_Pos.y >= WindowHeight / 2)
			m_Live = false;
	}
	else {
		if (m_Pos.x - m_Size / 2 <= -WindowWidth / 2 || m_Pos.x + m_Size / 2 >= WindowWidth / 2) {
			m_moveDir.x *= -1.0f;

			if (m_moveDir.x > 0) {
				m_aniState = 1;
				m_CurrYSeq = 0;
			} 
			else {
				m_aniState = -1;
				m_CurrYSeq = m_divide;
			}
		}
			
		if (m_Pos.y - m_Size / 2 <= -WindowHeight / 2 || m_Pos.y + m_Size / 2 >= WindowHeight / 2)
			m_moveDir.y *= -1.0f;
	}
}

void Objects::setminusLife(int tmp)
{ 
	m_Life -= tmp; 
	m_LifeGuage = (float)m_Life / (float)m_FullLife;
	sprintf_s(m_buf, "%d/%d", (int)m_Life, (int)m_FullLife);
}

void Objects::OnPrepareRender()
{

}

void Objects::Render(Renderer& g_Renderer)
{
	if (m_Live) {
		OnPrepareRender();
		if(m_texIndex==-1)
			g_Renderer.DrawSolidRect(m_Pos.x, m_Pos.y, m_Pos.z, m_Size,
				m_Color.x, m_Color.y, m_Color.z, m_Color.w, m_RenderLevel);
		else {
			if (m_IsAnimate) {
				g_Renderer.DrawTexturedRectSeq(m_Pos.x, m_Pos.y, m_Pos.z, m_Size, m_Color.x, m_Color.y, m_Color.z, m_Color.w,
					m_texIndex, m_CurrXSeq, m_CurrYSeq, m_MaxXSeq, m_MaxYSeq, m_RenderLevel);
			}
			else {
				g_Renderer.DrawTexturedRect(m_Pos.x, m_Pos.y, m_Pos.z, m_Size,
					m_Color.x, m_Color.y, m_Color.z, m_Color.w, m_texIndex, m_RenderLevel);
			}
		}

		if (m_isLifeGuage) {
			g_Renderer.DrawSolidRectGauge(m_Pos.x, m_Pos.y + m_Size/2 + 15.0f, m_Pos.z, 
				m_Size, 12, m_teamColor.x, m_teamColor.y, m_teamColor.z, m_teamColor.w, m_LifeGuage, 0.0f);

			g_Renderer.DrawTextW(m_Pos.x-m_Size/2, m_Pos.y + m_Size / 2 + 10.0f, GLUT_BITMAP_HELVETICA_12, 1, 1, 1, m_buf);
		}
		if (m_IsPaticle) {
			g_Renderer.DrawParticle(m_Pos.x, m_Pos.y, m_Pos.z, m_Size, 1, 1, 1, 1, m_moveDir.x*-1, m_moveDir.y*-1, m_PaticleTexIndex, m_paticleTime, m_RenderLevel);
		}
	}
}

bool Objects::Update(float ElapsedTime)
{
	if (m_Live) {
		m_oobb->refrash(m_Pos);
		Animate();
		CrashCheck();
		Move(ElapsedTime);

		DWORD currTime = timeGetTime() *0.001f;

		
		if (m_IsPaticle) {
			m_paticleTime += 0.01f;
		}
		/*m_LifeTime -= 1*ElapsedTime;
		if (m_LifeTime < 0)
			m_Live = false;*/
		if (m_Life <= 0)
			m_Live = false;

		if (currTime - m_PrevTime > m_LimitTime) {
			m_PrevTime = currTime;
			return true;
		}

		return false;
	}

}

