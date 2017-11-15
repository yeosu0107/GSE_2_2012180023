#pragma once

#include "targetver.h"

#include <iostream>
#include <tchar.h>
#include <windows.h>
#pragma comment(lib, "winmm.lib") 

#define WindowWidth 500
#define WindowHeight 800



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

	void normalize() {
		float tmpx = x<0 ? x*-1 : x;
		float tmpy = y<0 ? y*-1 : y;

		float size = 1.0f / (tmpx + tmpy);
		x *= size;
		y *= size;
		//z *= size;
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
	float3 m_pos;
	float m_size;
	float m_collisonLen;

	float minX, maxX, minY, maxY;

	OOBB(float3 pos, float size) {
		m_size =size/2;
		m_collisonLen = (m_size*1.4f) * (m_size*1.4f); //1.4는 루트2의 근사값
		m_pos = pos;

		minX = m_pos.x - m_size; maxX = m_pos.x + m_size;
		minY = m_pos.y - m_size; maxY = m_pos.y + m_size;

	}
	bool collision(const OOBB& tmp) {
		float len = (tmp.m_pos.x - m_pos.x) * (tmp.m_pos.x - m_pos.x) +
			(tmp.m_pos.y - m_pos.y) * (tmp.m_pos.y - m_pos.y);

		if (len <= m_collisonLen || len<=tmp.m_collisonLen) {
			if (tmp.minX > maxX)
				return false;
			if (tmp.minY > maxY)
				return false;
			if (tmp.maxX < minX)
				return false;
			if (tmp.maxY < minY)
				return false;
			return true;
		}
		return false;
	}
	void refrash(float3 pos) {
		m_pos = pos;
		
		minX = m_pos.x - m_size; maxX = m_pos.x + m_size;
		minY = m_pos.y - m_size; maxY = m_pos.y + m_size;
	}
	
};


