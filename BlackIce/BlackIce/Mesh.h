#pragma once
#include "pch.h"

namespace BlackIceEngine
{
	class Mesh
	{
	private:
		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR4 color;
		};

	public:
		Mesh();
		virtual ~Mesh();

		void Init();
		void Shutdown();
		void Render();

		const int GetIndexCount() { return m_indexCount; }

	private:
		bool InitBuffers(ID3D11Device* device);
		void ShutdownBuffers();
		void RenderBuffers(ID3D11DeviceContext* context);

	private:
		int m_vertexCount;
		ID3D11Buffer* m_vertexBuffer;
		int m_indexCount;
		ID3D11Buffer* m_indexBuffer;
	};
}
