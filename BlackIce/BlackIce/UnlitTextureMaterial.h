#pragma once
#include "pch.h"
#include "Texture.h"

namespace BlackIceEngine
{
	class UnlitTextureMaterial
	{
	private:
		struct MatrixBufferType
		{
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;
		};

	public:
		UnlitTextureMaterial();
		virtual ~UnlitTextureMaterial();

		void Init();
		void Shutdown();
		void Render(int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection);

	private:
		bool InitShader(ID3D11Device* device, HWND hwnd, WCHAR* shader);
		void ShutdownShader();
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

		bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection);
		void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		ID3D11Buffer* m_matrixBuffer;

		ID3D11SamplerState* m_sampleState;
		Texture* m_texture;
	};
}
