#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define WindowWidth 500
#define WindowHeight 500

struct float3
{
	float x, y, z;
	float3() {
		x = 0; y = 0; z = 0;
	}
	float3(float a, float b, float c) {
		x = a; y = b; z = c;
	}
	float3 operator+=(const float3& num) {
		x = x + num.x;
		y = y + num.y;
		z = z + num.z;
		return float3(x, y, z);
	}
	float3 operator-=(const float3& num) {
		x = x - num.x;
		y = y - num.y;
		z = z - num.z;
		return float3(x, y, z);
	}
	float3 operator*(const float& num) {
		float tx = x*num;
		float ty = y*num;
		float tz = z*num;
		return float3(tx, ty, tz);
	}
	bool operator>=(const float3& num) const{
		if (x > num.x && y > num.y)
			return true;
		return false;
	}
	bool operator<=(const float3& num) const{
		if (x < num.x && y < num.y)
			return true;
		return false;
	}
	bool operator>(const float3& num) const {
		return(*this <= num);
	}
	bool operator<(const float3& num) const {
		return(*this >= num);
	}
};

struct float4
{
	float x, y, z, w;
	float4(float a, float b, float c, float d) {
		x = a; y = b; z = c; w = d;
	}
};

struct OOBB
{
	//float3 Edge[4];
	float3 m_pos;
	float m_size;
	float m_collisonLen;
	OOBB(float3 pos, float size) {
		m_size =size/2;
		m_collisonLen = m_size*m_size;
		m_pos = pos;
		/*Edge[0] = float3(pos.x - m_size, pos.y - m_size, 0);
		Edge[1] = float3(pos.x + m_size, pos.y - m_size, 0);
		Edge[2] = float3(pos.x - m_size, pos.y + m_size, 0);
		Edge[3] = float3(pos.x + m_size, pos.y + m_size, 0);*/
	}
	bool collision(const OOBB& tmp) {
		float len = (tmp.m_pos.x - m_pos.x) * (tmp.m_pos.x - m_pos.x) +
			(tmp.m_pos.y - m_pos.y) * (tmp.m_pos.y - m_pos.y);

		if (len <= m_collisonLen)
			return true;
		return false;
	}
	void refrash(float3 pos) {
		m_pos = pos;
		/*Edge[0] = float3(pos.x - m_size, pos.y - m_size, 0);
		Edge[1] = float3(pos.x + m_size, pos.y - m_size, 0);
		Edge[2] = float3(pos.x - m_size, pos.y + m_size, 0);
		Edge[3] = float3(pos.x + m_size, pos.y + m_size, 0);*/
	}
	
};


