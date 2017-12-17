#pragma once
#include "pch.h"

namespace BlackIceEngine
{
	class Light
	{
	public:
		Light();
		virtual ~Light();

		void SetDiffuseColor(float r, float g, float b, float a = 1) { m_diffuseColor = D3DXVECTOR4(r, g, b, a); }
		D3DXVECTOR4 GetDiffuseColor() const { return m_diffuseColor; }

		void SetDirection(float x, float y, float z) { D3DXVec3Normalize(&m_direction, &D3DXVECTOR3(x, y, z)); }
		D3DXVECTOR3 GetDirection() const { return m_direction; }
	private:
		D3DXVECTOR4 m_diffuseColor;
		D3DXVECTOR3 m_direction;
	};
}
