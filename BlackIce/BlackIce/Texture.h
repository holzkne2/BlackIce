#pragma once
#include "pch.h"
#include <d3dx11tex.h>

namespace BlackIceEngine
{
	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		void Init(WCHAR* filename);
		void Shutdown();

		ID3D11ShaderResourceView* GetTexture() { return m_texture; }

	private:
		ID3D11ShaderResourceView* m_texture;
	};
}
