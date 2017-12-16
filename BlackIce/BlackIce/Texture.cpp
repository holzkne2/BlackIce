#include "stdafx.h"
#include "Texture.h"
#include "BlackIceEngine.h"

using namespace BlackIceEngine;

Texture::Texture()
{
	m_texture = 0;
}


Texture::~Texture()
{
}

void Texture::Init(WCHAR* filename)
{
	ID3D11Device* device = Engine::Get().GetDeviceResources()->GetD3DDevice();
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
}

void Texture::Shutdown()
{
	if (m_texture) {
		m_texture->Release();
		m_texture = 0;
	}
}